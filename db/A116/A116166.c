/*
 * A116166  a(n) = 8^n * n*(n+1).
 * https://oeis.org/A116166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * n*(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * ((seq_t)n + 1);
    seq_t pow8 = seq_pow((seq_t)8, n);
    return prod * pow8;
}
