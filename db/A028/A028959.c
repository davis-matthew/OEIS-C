/*
 * A028959  Theta series of quadratic form with Gram matrix [ 2, 1; 1, 12 ].
 * https://oeis.org/A028959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) in ℤ² : x² + x·y + 6·y² = n }
 * Range:     u8 n <= 15, u16 n <= 3854, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts integer solutions to the quadratic form with Gram matrix [2,1;1,12] */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t nseq = (seq_t)n;                     /* convert index to seq_t for arithmetic */

    /* bound for |y|: 23*y² ≤ 4*n  ⇒  |y| ≤ sqrt(4*n / 23) */
    seq_t t       = (seq_t)4 * nseq / (seq_t)23;    /* integer division truncates */
    seq_t y_max_sq = floor_sqrt(t);
    long long y_max = (long long)y_max_sq;          /* safe for n up to typical limits */

    for (long long y = -y_max; y <= y_max; ++y)
    {
        long long abs_y = y >= 0 ? y : -y;
        seq_t yy  = (seq_t)abs_y * (seq_t)abs_y;    /* y² */

        seq_t D = (seq_t)4 * nseq - (seq_t)23 * yy; /* discriminant 4*n - 23*y² */
        if (D < (seq_t)0) continue;                 /* negative discriminant → no solutions */

        seq_t k = floor_sqrt(D);
        if (k * k != D) continue;                  /* D must be a perfect square */

        /* parity check: (-y ± k) must be even ⇒ k and y have same parity */
        if (((k & (seq_t)1) != ((seq_t)(y & 1)))) continue;

        /* count solutions in x */
        if (k == 0)               /* double root: one x for this y */
            result += 1;
        else                      /* two distinct x values */
            result += 2;
    }

    return result;
}
