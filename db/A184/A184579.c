/*
 * A184579  a(n) = floor((n-1/3)*(2+sqrt(2))), complement of A184578.
 * https://oeis.org/A184579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/3)*(2+sqrt(2)))
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 5805, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid overflow */
    seq_t n_seq = (seq_t)n;

    /* m = 3*n - 1  (m = 3n - 1) */
    seq_t m = n_seq * 3 - 1;

    /* q = floor(2*m/3)  (the integer part of 2m/3) */
    seq_t q = (2 * m) / 3;

    /* Compute s = floor(sqrt(2*m^2)) = floor(m*sqrt(2)) */
    seq_t two_mm = 2 * m * m;
    seq_t s = floor_sqrt(two_mm);    /* s = floor(m*sqrt(2)) */

    /* t = floor(s/3)  and the remainder r = s mod 3 */
    seq_t t = s / 3;
    seq_t r = s % 3;

    /* the extra 1 occurs iff r == 2  (see analysis) */
    seq_t b = (r == 2) ? 1 : 0;

    /* a(n) = q + t + b */
    return q + t + b;
}
