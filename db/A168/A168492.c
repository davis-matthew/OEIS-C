/*
 * A168492  Sequence with Hankel transform equal to 3^floor(n^2/2).
 * https://oeis.org/A168492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Figure 8.1 from A168492
 * Formula: (n+1)*a(n) = 2*(8*n-3)*a(n-1) - 4*(19*n-31)*a(n-2) + 48*(2*n-5)*a(n-3)
 * Initial values: a(0)=1, a(1)=1, a(2)=2
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;

    /* initialise cache with first three terms */
    if (!cache) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last = 2;
    }

    /* return immediately if already computed */
    if (n <= last)
        return cache[(size_t)n];

    /* ensure capacity for index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing entries up to n using the recurrence */
    for (idx_t k = last + 1; k <= n; ++k) {
#if defined(__cplusplus)
        /* C++: seq_t is assumed to be a big‑integer class; 
           operators * and / are overloaded appropriately */
        seq_t term1 = cache[(size_t)(k - 1)] * (seq_t)(2 * ((8 * k) - 3));
        seq_t term2 = cache[(size_t)(k - 2)] * (seq_t)(4 * ((19 * k) - 31));
        seq_t term3 = cache[(size_t)(k - 3)] * (seq_t)(48 * ((2 * k) - 5));
        seq_t num = term1 - term2 + term3;
        cache[(size_t)k] = num / (k + 1);
#else
        /* C: use 128‑bit intermediate when available to avoid overflow */
#if defined(__SIZEOF_INT128__)
        typedef unsigned __int128 u128;
        u128 t1 = 2 * ((8 * k) - 3);
        u128 t2 = 4 * ((19 * k) - 31);
        u128 t3 = 48 * ((2 * k) - 5);
        u128 m1 = (u128)cache[(size_t)(k - 1)];
        u128 m2 = (u128)cache[(size_t)(k - 2)];
        u128 m3 = (u128)cache[(size_t)(k - 3)];
        u128 num = m1 * t1 + m3 * t3 - m2 * t2;
        cache[(size_t)k] = (seq_t)(num / (u128)(k + 1));
#else
        /* fallback: multiply and divide in seq_t (may overflow if seq_t is small) */
        seq_t term1 = cache[(size_t)(k - 1)] * (seq_t)(2 * ((8 * k) - 3));
        seq_t term2 = cache[(size_t)(k - 2)] * (seq_t)(4 * ((19 * k) - 31));
        seq_t term3 = cache[(size_t)(k - 3)] * (seq_t)(48 * ((2 * k) - 5));
        seq_t num = term1 - term2 + term3;
        cache[(size_t)k] = num / (k + 1);
#endif
#endif
    }

    last = n;
    return cache[(size_t)n];
}
