/*
 * A132655  Pentagonal number P(n) raised to power P(n), for P(n) > 0.
 * https://oeis.org/A132655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = P(n)^P(n), where P(n) = n*(3*n-1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    seq_t p = (seq_t)n * ((seq_t)3 * n - 1) / 2;
    return big_pow(p, p);
}
