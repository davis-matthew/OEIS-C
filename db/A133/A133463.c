/*
 * A133463  Partial sums of the sequence that starts with 2 and is followed by
 *          A111575.
 * https://oeis.org/A133463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n mod 4 + 2) * 3^{⌊n/4⌋} for all n ≥ 0, which is
 *            equivalent to the OEIS specification: a(4n) = 2*A111575(4n),
 *            a(4n+1) = 3*A111575(4n+1), a(4n+2) = 4*A111575(4n+2), a(4n+3) =
 *            5*A111575(4n+3)
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t n_div = n / 4;
    seq_t pow3  = big_pow((seq_t)3, (seq_t)n_div);
    seq_t mult  = (seq_t)((n % 4) + 2);
    return pow3 * mult;
}
