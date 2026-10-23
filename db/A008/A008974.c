/*
 * A008974  Triangular number t(n) raised to power t(n).
 * https://oeis.org/A008974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2)^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return big_pow(t, t);
}
