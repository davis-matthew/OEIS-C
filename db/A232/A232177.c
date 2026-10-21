/*
 * A232177  Least positive k such that triangular(n) + triangular(k) is a
 *          square.
 * https://oeis.org/A232177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that T(n)+T(k) is a perfect square
 *            /* with offset 0; T(m) = m*(m+1)/2
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* triangular number for n */
    seq_t Tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* For n = 0 the answer is 1; for n = 1 answer is 2;
       for n > 1 the bound is n-1, as guaranteed by T(n)+T(n-1) = n^2. */
    idx_t max_k = (n > 1) ? (n - 1) : ((n == 1) ? 2 : 1);

    /* T(1) = 1 */
    seq_t Tk = 1;

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t sum = Tn + Tk;
        seq_t s   = sqrt_floor(sum);
        if (s * s == sum)
            return k;

        /* advance to the next triangular number */
        if (k < max_k)
            Tk += k + 1;
    }

    /* Should never happen, but keep compiler happy */
    return max_k;
}
