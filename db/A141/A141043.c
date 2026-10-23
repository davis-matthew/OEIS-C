/*
 * A141043  Number of sequences of length n whose terms are positive integers
 *          less than or equal to n in which the i-th term is greater than
 *          both the (i-2)nd and (i-3)rd terms.
 * https://oeis.org/A141043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∑_{m=0}^{⌊n/2⌋} binom(n - m, m) * binom(n + m, m)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for n=0, not used by OEIS offset 1 */
    idx_t limit = n / 2;
    seq_t sum = 0;
    for (idx_t m = 0; m <= limit; ++m) {
        seq_t b1 = binom(n - m, m);   /* binom(n - m, m) */
        seq_t b2 = binom(n + m, m);   /* binom(n + m, m) */
        sum += b1 * b2;
    }
    return sum;
}
