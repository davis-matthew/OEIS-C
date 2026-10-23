/*
 * A117302  Number of cases in which the first player gets killed in a Russian
 *          roulette game when 7 players use a gun with n chambers and the
 *          number of the bullets can be from 1 to n. In the game they do not
 *          rotate the cylinder after the game starts.
 * https://oeis.org/A117302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117302 – Number of ways the first player gets killed
 *
 *  Formula:  a(n) = floor(2^(n+6) / 127)
 *  Recurrence:  a(n) = 2*a(n-1) + a(n-7) - 2*a(n-8)
 *  Initial values:  a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=16, a(6)=32,
 *                   a(7)=64, a(8)=129
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

/* static memoisation data */
static seq_t *cache = NULL;      /* 1‑based indexing, cache[0] unused */
static size_t cache_size = 0;    /* allocated size (n+1) */
static idx_t last_computed = 0;  /* farthest computed n */

/* ensure cache holds values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n <= last_computed)
        return; /* already computed */

    /* allocate or grow array to size n+1 (index 0 unused) */
    size_t needed = (size_t)n + 1;
    if (!cache) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_size = needed;
    } else if (needed > cache_size) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_size = needed;
    }

    /* compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 8) {
            /* a(i) = floor( 2^(i+6) / 127 ) ; safe shift for i≤8 */
            seq_t pow2 = (seq_t)1ULL << (i + 6);
            cache[i] = pow2 / (seq_t)127;
        } else {
            /* recurrence: a(i) = 2*a(i-1) + a(i-7) - 2*a(i-8) */
            cache[i] = cache[i-1] * 2
                     + cache[i-7]
                     - cache[i-8] * 2;
        }
    }
    last_computed = n;
}

/* sequence entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
