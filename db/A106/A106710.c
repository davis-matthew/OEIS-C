/*
 * A106710  Number of words with n letters from an alphabet of size 26 with at
 *          least two equal consecutive letters.
 * https://oeis.org/A106710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26^n - 26*25^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   pow_seq_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_base.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not part of the sequence but safe */
    seq_t pow26 = pow_seq_base((seq_t)26, n);
    seq_t pow25 = pow_seq_base((seq_t)25, n - 1);
    return pow26 - (seq_t)26 * pow25;
}
