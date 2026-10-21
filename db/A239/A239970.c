/*
 * A239970  Least positive k such that triangular(k) + triangular(n+k) is a
 *          triangular number (A000217).
 * https://oeis.org/A239970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least positive k such that * T(k) + T(n+k) is a
 *            triangular number, * where T(x) = x*(x+1)/2. * From the equation
 *            1+4*(2k^2+2k+n^2+2nk+n) = m^2, * we check for the smallest k>0
 *            with this discriminant a perfect square
 * Range:     u8 n <= 0, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;          /* a(0) = 2 */

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; ++k)               /* search for the minimal k */
    {
        seq_t k_seq = (seq_t)k;
        seq_t D = 8 * k_seq * k_seq
                + 4 * ((2 * k_seq + (seq_t)1) * n_seq)
                + 4 * n_seq * n_seq
                + 8 * k_seq
                + (seq_t)1;

        seq_t r = floor_sqrt(D);           /* integer square root of D */
        if (r * r == D)                    /* perfect square? */
            return k_seq;                  /* found minimal k */
    }
}
