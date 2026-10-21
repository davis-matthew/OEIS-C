/*
 * A081477  Complement of A086377.
 * https://oeis.org/A081477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n + floor(n*sqrt(2))) / sqrt(2))
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only. */

#include "../utils/floor_sqrt.h"

/* Compute A081477: complement of A086377 */
seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* b = floor(n*sqrt(2)) = floor_sqrt(2 * n^2) */
    seq_t two_n_sq = (seq_t)2 * sn * sn;
    seq_t b = floor_sqrt(two_n_sq);

    /* X = n + b */
    seq_t X = sn + b;

    /* Y = ceil(X^2 / 2) */
    seq_t Xsq = X * X;
    seq_t Y = Xsq / 2;
    if ((Xsq % 2) != 0) Y += 1;

    /* a(n) = ceil_sqrt(Y) = floor_sqrt(Y) + (floor_sqrt(Y)^2 < Y) */
    seq_t sq = floor_sqrt(Y);
    if (sq * sq == Y)
        return sq;
    else
        return sq + 1;
}
