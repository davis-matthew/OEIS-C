/*
 * A176379  The smallest prime q which stays prime through at least two
 *          iterations of q -> := 2 * q + prime(n+1).
 * https://oeis.org/A176379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A176379: for a given index n≥1, find the smallest prime q such that
   1) 2q + prime(n+1) is prime,
   2) 4q + 3·prime(n+1) is also prime.
   The function caches computed values so repeated calls are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
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

static seq_t *cache = NULL;       /* cache[n-1] holds a(n) for n>=1 */
static idx_t cache_len = 0u;      /* number of cached entries */

/* Ensure the cache has room for at least n entries. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    seq_t *new_cache = mem_resize(seq_t, cache, n);
    if (!new_cache) {
        abort(); /* allocation failure */
    }

    /* Initialize newly allocated space to zero */
    for (idx_t i = cache_len; i < n; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_len = n;
}

/* Compute a(n) if not already cached. */
static void compute_at(idx_t n)
{
    /* prime(n+1) needed for the definition */
    uint32_t p = get_prime(n + 1u);   /* fits in 32 bits */
    seq_t prime_n1 = (seq_t)p;

    seq_t q = 2;  /* start with the smallest prime */
    while (true) {
        seq_t f1 = 2 * q + prime_n1;        /* first iteration */
        if (!is_prime_big(f1)) {
            /* advance q to the next prime */
            uint64_t q_next = next_prime((uint64_t)q + 1u);
            q = (seq_t)q_next;
            continue;
        }

        seq_t f2 = 2 * f1 + prime_n1;        /* second iteration */
        if (is_prime_big(f2)) {
            cache[n - 1] = q;
            return;
        }

        uint64_t q_next = next_prime((uint64_t)q + 1u);
        q = (seq_t)q_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0; /* undefined for n < 1; safety value */

    ensure_cache(n);
    if (cache[n - 1] != 0u)
        return cache[n - 1];

    compute_at(n);
    return cache[n - 1];
}
