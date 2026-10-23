/*
 * A131498  For D_2 type groups as polyhedra:
 *          {F,V,E,dimension}->{n+2,2*n,3*n,2*n*(2*n-1)/2} such that Euler's
 *          equation is true: V=E-F+2.
 * https://oeis.org/A131498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) for n≥13
   Initial values: a(1)=3, a(2)=2, a(3)=3, a(4)=1, a(5)=4, a(6)=4,
   a(7)=6, a(8)=6, a(9)=5, a(10)=6, a(11)=9, a(12)=15
   The cache size is set large enough for n up to about 10 000. */

#include <stddef.h>

#define CACHE_CAP 20000            /* buffer for computed values */

/* Initial terms a(1) … a(12) */
static const seq_t init[12] = {
    (seq_t)3, (seq_t)2, (seq_t)3, (seq_t)1,
    (seq_t)4, (seq_t)4, (seq_t)6, (seq_t)6,
    (seq_t)5, (seq_t)6, (seq_t)9, (seq_t)15
};

/* Storage for computed values, indexed from 1. */
static seq_t cache[CACHE_CAP + 1] = {0};
static size_t cache_max = 0;     /* largest n for which cache[n] is valid */

/* Compute the n‑th term of A131498. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety guard */

    size_t N = (size_t)n;
    if (N > CACHE_CAP) {                 /* n is larger than allocated cache */
        /* For the purposes of this task, we only support n up to CACHE_CAP. */
        /* If a larger n is requested, fallback to an on‑the‑fly computation
           without caching. */
        size_t i;
        if (N <= 12) return init[N-1];

        /* Compute using recurrence up to N, but only keep the last 12 terms. */
        seq_t t[13];                     /* indices 0..12, we use 1..12 */
        for (i = 1; i <= 12; ++i) t[i] = init[i-1];
        for (i = 13; i <= N; ++i) {
            t[i-12] = (seq_t)3 * t[i-16] - (seq_t)3 * t[i-20] + t[i-24];
        }
        return t[N-12];
    }

    /* Ensure base values are cached. */
    if (cache_max < 12) {
        size_t i;
        for (i = 1; i <= 12 && i <= N; ++i) cache[i] = init[i-1];
        cache_max = (N >= 12) ? 12 : N;
    }

    /* Extend cache to N. */
    if (N > cache_max) {
        size_t i;
        for (i = cache_max + 1; i <= N; ++i) {
            cache[i] = (seq_t)3 * cache[i-4]
                     - (seq_t)3 * cache[i-8]
                     + cache[i-12];
        }
        cache_max = N;
    }

    return cache[N];
}
