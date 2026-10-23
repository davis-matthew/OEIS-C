/*
 * A151025  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, 0, 0), (1, -1,
 *          0), (1, -1, 1), (1, 1, 0), (1, 1, 1)}.
 * https://oeis.org/A151025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σₖ₌⌈n/2⌉ⁿ ((2k−n+1)/(k+1) · C(n,k)) · C(k,⌊k/2⌋) · 2ᵏ
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary arrays of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *pow2   = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *central = (seq_t*)malloc(len * sizeof(seq_t));
    if (!pow2 || !central) abort();

    /* Pre‑compute powers of two: 2ᵏ */
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i-1] * (seq_t)2;

    /* Pre‑compute central binomials C(k,⌊k/2⌋) with a simple recurrence */
    central[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = k / 2;                     /* floor(k/2)   */
        central[k] = central[k-1] * (seq_t)k / (seq_t)(k - m);
    }

    /* Evaluate the sum */
    seq_t sum = 0;
    idx_t k_start = (n + 1) >> 1;            /* ceil(n/2)    */

    /* Compute initial binomial C(n, k_start) */
    seq_t binom = 1;
    if (k_start > 0) {
        for (idx_t t = 1; t <= k_start; ++t)
            binom = binom * (seq_t)(n - k_start + t) / (seq_t)t;
    }

    for (idx_t k = k_start; k <= n; ++k) {
        if (k > k_start)                     /* update binomial to C(n,k) */
            binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* Ballot number G(n,k) = (2k - n + 1)/(k+1) * C(n,k) */
        seq_t num = binom * (seq_t)(2*k - n + 1);
        seq_t G   = num / (seq_t)(k + 1);

        /* Combine all factors */
        seq_t term = G * central[k];
        term = term * pow2[k];
        sum += term;
    }

    free(pow2);
    free(central);
    return sum;
}
