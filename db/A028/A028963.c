/*
 * A028963  Theta series of quadratic form with Gram matrix [ 2, 1, 0; 1, 3,
 *          1; 0, 1, 5 ].
 * https://oeis.org/A028963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Theta series of quadratic form with Gram matrix 
   [ 2, 1, 0; 1, 3, 1; 0, 1, 5 ].  Count integer vectors (x,y,z)
   satisfying 2x^2 + 2xy + 3y^2 + 2yz + 5z^2 = n. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Bound on |y| and |z|: for any positive definite form
       |variable| ≤ sqrt(n).  This is safe for all n >= 0. */
    idx_t bound = (idx_t)floor_sqrt((seq_t)n);   /* floor(sqrt(n))   */
    seq_t count = 0;

    /* Loop over y in [‑bound .. bound] */
    for (idx_t yi = 0; yi <= 2*bound; ++yi)
    {
        long long y = (long long)( (long long)yi - (long long)bound );   /* signed y */
        seq_t y_sq = (seq_t)y * (seq_t)y;

        /* Loop over z in [‑bound .. bound] */
        for (idx_t zi = 0; zi <= 2*bound; ++zi)
        {
            long long z = (long long)( (long long)zi - (long long)bound ); /* signed z */
            seq_t z_sq = (seq_t)z * (seq_t)z;
            seq_t yz   = (seq_t)y * (seq_t)z;

            /* Compute the quadratic part that does not involve x */
            seq_t c = (seq_t)3 * y_sq + (seq_t)2 * yz + (seq_t)5 * z_sq;

            /* Discriminant D = y^2 + 2*(n – c).  Compute it without negative
               intermediate values to avoid underflow for unsigned seq_t. */
            seq_t u       = y_sq + (seq_t)2 * (seq_t)n;   /* y^2 + 2n */
            seq_t two_c   = c << 1;                      /* 2c */
            if (two_c > u) continue;                    /* D would be negative */
            seq_t D = u - two_c;                         /* D = y^2 + 2(n – c) */

            /* D must be a perfect square. */
            seq_t s = floor_sqrt(D);
            if (s * s != D) continue;

            /* Parity condition: x = (-y ± s)/2 must be integer,
               i.e., s and y must have the same parity. */
            uint64_t parity_s = (uint64_t)s & 1u;               /* safe: s is small */
            uint64_t parity_y = ((uint64_t)(y & 1)) & 1u;       /* y may be negative */
            if (parity_s != parity_y) continue;

            /* Count solutions: two distinct x's if s ≠ 0, one if s = 0. */
            if (s == 0)      count += 1;
            else             count += 2;
        }
    }
    return count;
}
