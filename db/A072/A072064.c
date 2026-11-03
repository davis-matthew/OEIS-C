/*
 * A072064  Least k>0 such that prime(n)+k*n is prime.
 * https://oeis.org/A072064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 3133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9890 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A072064: a(n) = least k>0 such that prime(n)+k*n is prime.  */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>
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

static seq_t *cache = NULL;     /* Cache for already computed values. */
static size_t cache_cap = 0;    /* Current capacity of the cache buffer. */

/*  Return the least k>0 with prime(n)+k*n prime.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* offset 1 – undefined for n=0 */

    /* Ensure the cache is big enough. */
    if ((size_t)n > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* allocation failed – fall back to direct calculation */
            goto compute;
        }
        cache = tmp;
        cache_cap = new_cap;
        /* Zero any newly allocated entries. */
        if (new_cap > old_cap)
            mem_set(cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    }

    /* If we have this value already, return it. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

compute:
    {
        seq_t p   = get_prime(n);        /* prime(n)          */
        seq_t step = (seq_t)n;           /* n                 */
        seq_t cand = p + step;           /* k = 1 candidate  */
        idx_t k = 1;

        while (!is_prime_big(cand)) {
            cand += step;
            ++k;
        }

        /* Store the result in cache if possible. */
        if ((size_t)n <= cache_cap)
            cache[n - 1] = (seq_t)k;

        return (seq_t)k;
    }
}
