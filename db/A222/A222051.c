/*
 * A222051  Central terms in rows of triangle A220178.
 * https://oeis.org/A222051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{r=0}^{⌊n/2⌋} C(3n, n) · C(2n, r) · C(2n–r, n–2r)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* total sum of terms */
    seq_t sum = 0;

    /* precomputed constants */
    seq_t A = binomial((idx_t)(3 * n), (idx_t)n);          /* C(3n, n)      */
    seq_t B = 1;                                           /* C(2n, 0)      */
    seq_t C = binomial((idx_t)(2 * n), (idx_t)n);          /* C(2n, n)      */

    idx_t r_max = n / 2;                                   /* floor(n/2)    */

    for (idx_t r = 0; r <= r_max; ++r)
    {
        /* add current term */
        sum += A * B * C;

        /* no further updates for the last r */
        if (r == r_max) break;

        /* update B = C(2n, r+1) from C(2n, r) */
        B *= (seq_t)(2 * n - r);   /* multiply by (2n – r) */
        B /= (seq_t)(r + 1);       /* divide by (r + 1) */

        /* update C for next r */
        idx_t next = r + 1;
        seq_t numer = (seq_t)(n - 2 * next + 2) * (seq_t)(n - 2 * next + 1);
        seq_t denom = (seq_t)(2 * n - next + 1) * (seq_t)(n + next);
        C *= numer;
        C /= denom;
    }

    return sum;
}
