/*
 * A196836  a(n) = (1^n + 2^n +3^n + 4^n)/2.
 * https://oeis.org/A196836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + 2^n + 3^n + 4^n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;
    seq_t sum = 1;                 /* 1^n = 1 */
    sum += big_pow((seq_t)2, exp);  /* + 2^n */
    sum += big_pow((seq_t)3, exp);  /* + 3^n */
    sum += big_pow((seq_t)4, exp);  /* + 4^n */
    return sum / 2;                /* divide by 2 */
}
