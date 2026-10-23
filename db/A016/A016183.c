/*
 * A016183  Expansion of g.f. 1/((1-7*x)*(1-11*x)).
 * https://oeis.org/A016183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^(n+1) - 7^(n+1))/4 (recurrence: a(0)=1, a(1)=18,
 *            a(n)=18*a(n-1)-77*a(n-2))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t exp = (seq_t)(n + 1);
    seq_t pow11 = big_pow((seq_t)11, exp);
    seq_t pow7  = big_pow((seq_t)7, exp);
    return (pow11 - pow7) / (seq_t)4;
}
