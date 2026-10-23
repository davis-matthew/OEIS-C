/*
 * A131025  Antidiagonal sums of triangular array T: T(j,1) = 1 for ((j-1) mod
 *          6) < 3, else 0; T(j,k) = T(j-1,k-1) + T(j-1,k) for 2 <= k <= j.
 * https://oeis.org/A131025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 6*a(n-3) - 8*a(n-4) + 14*a(n-5) +
 *            a(n-6) - 15*a(n-7) + 9*a(n-8) + 6*a(n-9) - 6*a(n-10) (n ≥ 11)
 *            Initial values: a(1)=1, a(2)=1, a(3)=3, a(4)=2, a(5)=5, a(6)=3,
 *            a(7)=9, a(8)=6, a(9)=16, a(10)=11 The sequence is the
 *            antidiagonal sums of the triangular array T
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 66, u64 n <= 128,
 *            u128 n <= 258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based sequence stored 0‑indexed */
static idx_t max_computed = 0;      /* largest n that has been computed */

seq_t a(idx_t n) {
    /* Handle trivial cases by returning early when possible. */
    if (n == 0) return (seq_t)0;          /* offset is 1, but callers may pass 0 */
    /* Ensure the cache is initialized with the first 10 terms. */
    if (max_computed == 0) {
        const size_t init_cap = 10;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* memory error – abort gracefully */
        cache[0] = 1;  cache[1] = 1;  cache[2] = 3;  cache[3] = 2;
        cache[4] = 5;  cache[5] = 3;  cache[6] = 9;  cache[7] = 6;
        cache[8] = 16; cache[9] = 11;
        max_computed = 10;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed) return cache[n-1];

    /* Grow the cache to accommodate the requested index. */
    size_t needed = (size_t)n;
    size_t cap = max_computed;
    if (cap < needed) {
        cap = needed;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* memory error – abort gracefully */
    }

    /* Compute additional terms using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Indices are 1‑based; cache is 0‑based. */
        seq_t pos = cache[i-2];                       /* a(i-1)   */
        pos += 5 * cache[i-3];                       /* +5 a(i-2)*/
        pos += 14 * cache[i-6];                      /* +14 a(i-5)*/
        pos += cache[i-7];                           /* + a(i-6) */
        pos += 9 * cache[i-9];                       /* +9 a(i-8) */
        pos += 6 * cache[i-10];                      /* +6 a(i-9) */

        seq_t neg = 6 * cache[i-4];                  /* 6 a(i-3) */
        neg += 8 * cache[i-5];                      /* +8 a(i-4) */
        neg += 15 * cache[i-8];                     /* +15 a(i-7)*/
        neg += 6 * cache[i-11];                     /* +6 a(i-10)*/

        cache[i-1] = pos - neg;                      /* a(i) = pos - neg */
    }
    max_computed = n;
    return cache[n-1];
}
