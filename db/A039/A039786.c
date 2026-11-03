/*
 * A039786  phi(a(n)) is equal to the multiplicative projection of (a(n)-1).
 * https://oeis.org/A039786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer a ≥ 2 such that φ(a) equals the
 *            multiplicative projection of (a−1); a(1)=2, a(2)=3, a(3)=5,…
 * Range:     u8 n <= 34, u16 n <= 3688, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8852 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of 9 and all primes p for which
   multproj(p−1)=p−1, where multproj(m)=∏(p_i·e_i) over the
   prime factorisation m=∏p_i^e_i.  The code searches sequentially
   and memoises the results, storing them in a static array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true iff the multiplicative projection of m equals m.  */
static bool multproj_equals_m(uint64_t m)
{
    if (m <= 1) return true; /* for a=2 : m=1, trivially true */

    uint64_t tmp  = m;
    uint64_t prod = 1;

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            prod *= (p * e);
            if (prod > m) return false;   /* early rejection */
        }
    }
    if (tmp > 1) prod *= (tmp * 1); /* remaining prime factor */

    return prod == m;
}

static seq_t *cache   = NULL;   /* memoised a(n) terms               */
static size_t cache_sz = 0;     /* number of cached terms            */
static size_t cache_cap = 0;     /* allocated capacity of cache array */

/* Extend the cache until it holds at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_sz >= (size_t)n) return;   /* already enough terms */

    /* Grow the allocation if necessary. */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 64);
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* The next candidate to test.  */
    static seq_t cand = (seq_t)2;   /* start from 2 */

    while (cache_sz < (size_t)n)
    {
        bool ok = false;

        if (cand == (seq_t)9)
        {
            ok = true;   /* 9 is a special case */
        }
        else if (is_prime_big(cand))
        {
            uint64_t m = (uint64_t)cand - 1ULL;
            ok = multproj_equals_m(m);
        }

        if (ok)
        {
            cache[cache_sz++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below the offset */
    ensure_terms(n);
    return cache[n - 1];
}
