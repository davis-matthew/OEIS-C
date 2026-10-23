/*
 * A155656  a(n) = 9^n + 7^n - 1.
 * https://oeis.org/A155656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 7^n - 1 * Alternate recurrence: a(0)=1, a(1)=15, *
 *            a(n)=16*a(n-1)-63*a(n-2)-48 for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 9^n and 7^n using the utility big_pow.
       The exponent is passed as seq_t to match big_pow's signature. */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t sum  = pow9 + pow7;
    return sum - (seq_t)1;
}
