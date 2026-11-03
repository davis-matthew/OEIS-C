/*
 * A224493  Smallest k such that k*2*p(n)^2+1 is prime.
 * https://oeis.org/A224493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that k*2*p(n)^2 + 1 is prime,
 *            where p(n) is the n‑th prime. For n = 0 the function returns 0
 *            (offset not defined). The function memoizes results, so calls in
 *            any order or repeated indices remain fast
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 1305, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9873 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;    /* memoized sequence values */
static size_t  cache_cap = 0;  /* allocated size of cache            */
static idx_t   cache_used = 0; /* highest n that has been computed   */

/* helper: test primality of cand, choosing the fastest routine for
   the current seq_t size. */
static bool is_prime_candidate(seq_t cand)
{
    if (sizeof(seq_t) <= 8) {
        return is_prime_seq((uint64_t)cand);
    } else {
        return is_prime_big(cand);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1 is the first defined term */

    /* fast lookup if already computed */
    if (cache && n <= cache_used) {
        return cache[(size_t)n - 1];
    }

    /* grow cache if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort(); /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute p(n) = n-th prime */
    uint32_t p = get_prime((uint32_t)n);

    /* compute m = 2 * p^2  using a 128‑bit intermediate if seq_t is
       not arbitrary‑precision, to avoid overflow. */
    seq_t m;
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)2 * p * p;
        m = (seq_t)tmp;
    } else {
        m = (seq_t)2 * (seq_t)p * (seq_t)p;
    }

    /* search for the smallest k such that m*k+1 is prime */
    idx_t k = 1;
    for (;;) {
        unsigned __int128 prod = (unsigned __int128)m * (unsigned __int128)k;
        seq_t cand = (seq_t)(prod + 1);
        if (is_prime_candidate(cand)) {
            seq_t kseq = (seq_t)k;
            cache[(size_t)n - 1] = kseq;
            if (n > cache_used) cache_used = n;
            return kseq;
        }
        ++k;
    }
}
