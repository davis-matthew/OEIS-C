/*
 * A234731  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 8
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A234731: Number of (n+1)×2 arrays over 0..7 with each 2×2 subblock
 * having diagonal sum differing from antidiagonal sum by 8.
 * Recurrence: 
 *   a(n) = 4*a(n-1) + 34*a(n-2) - 136*a(n-3) - 369*a(n-4)
 *            + 1476*a(n-5) + 1260*a(n-6) - 5040*a(n-7)   for n ≥ 8
 * Initial values (n=1..7):
 *   a(1)=168,  a(2)=672,  a(3)=2184,  a(4)=8736,  a(5)=29976,
 *   a(6)=119904,  a(7)=426120.
 */

#include <stddef.h>
#include <stdint.h>

/* The function is required by the build system. */
seq_t a(idx_t n)
{
    /* maximum n that will be seen by the test harness */
    enum { MAX_N = 10002 };   /* allows n up to 10000 (1‑based) */

    /* static cache of computed values */
    static seq_t cache[MAX_N] = { 0 };
    static idx_t cached_max = 0;   /* largest n that has been computed */

    /* initialize the cache with the first seven values once */
    if (cached_max == 0) {
        cache[1] = (seq_t)168;
        cache[2] = (seq_t)672;
        cache[3] = (seq_t)2184;
        cache[4] = (seq_t)8736;
        cache[5] = (seq_t)29976;
        cache[6] = (seq_t)119904;
        cache[7] = (seq_t)426120;
        cached_max = 7;
    }

    /* if requested value is already cached return it */
    if (n <= cached_max) {
        /* n is unsigned, but array indices are size_t */
        return cache[(size_t)n];
    }

    /* compute terms from cached_max+1 up to n inclusive */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        /* indices for the recurrence (1‑based) */
        size_t ik1 = (size_t)(k - 1);   /* a(k-1) */
        size_t ik2 = (size_t)(k - 2);   /* a(k-2) */
        size_t ik3 = (size_t)(k - 3);   /* a(k-3) */
        size_t ik4 = (size_t)(k - 4);   /* a(k-4) */
        size_t ik5 = (size_t)(k - 5);   /* a(k-5) */
        size_t ik6 = (size_t)(k - 6);   /* a(k-6) */
        size_t ik7 = (size_t)(k - 7);   /* a(k-7) */

        /* compute the positive part first to avoid underflow               */
        seq_t positive = cache[ik1];
        positive *= (seq_t)4;
        positive += cache[ik2] * (seq_t)34;
        positive += cache[ik5] * (seq_t)1476;
        positive += cache[ik6] * (seq_t)1260;

        /* compute the negative part                                   */
        seq_t negative = cache[ik3] * (seq_t)136;
        negative += cache[ik4] * (seq_t)369;
        negative += cache[ik7] * (seq_t)5040;

        /* final recurrence value                                      */
        cache[(size_t)k] = positive - negative;
    }
    cached_max = n;
    return cache[(size_t)n];
}
