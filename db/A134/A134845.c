/*
 * A134845  Smallest number k not divisible by 10 such that k^2 contains
 *          exactly n zeros.
 * https://oeis.org/A134845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 32 a(3) = 448 if n even: a(n) = 10^(n/2 + 1) + 1 if n odd
 *            (n > 3): a(n) = 10^((n + 3)/2) + 4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)32;
    if (n == (idx_t)3) return (seq_t)448;
    if ((n & (idx_t)1) == (idx_t)0) {          // even n
        idx_t exp = n / (idx_t)2 + (idx_t)1;
        seq_t term = pow10(exp);
        term += (seq_t)1;
        return term;
    } else {                                    // odd n ≥ 5
        idx_t exp = (n + (idx_t)3) / (idx_t)2;
        seq_t term = pow10(exp);
        term += (seq_t)4;
        return term;
    }
}
