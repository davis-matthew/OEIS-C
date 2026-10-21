/*
 * A071588  Powers of 6 written backwards.
 * https://oeis.org/A071588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse_decimal(pow_seq((seq_t)6, n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   pow_seq, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/rev_decimal.h"

seq_t a(idx_t n)
{
    seq_t p = pow_seq((seq_t)6, n);
    return rev_decimal(p);
}
