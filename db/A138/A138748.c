/*
 * A138748  a(n) = (n+(n+1)) + (n*(n+1)) + (n^(n+1)).
 * https://oeis.org/A138748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+(n+1)) + (n*(n+1)) + (n^(n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t one = (seq_t)1;
    seq_t sum = nn + (nn + one);
    seq_t prod = nn * (nn + one);
    seq_t pow = big_pow(nn, (seq_t)(n + 1));
    return sum + prod + pow;
}
