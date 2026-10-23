/*
 * A183663  Number of (n+1) X (n+1) 0..6 arrays with every 2 X 2 subblock
 *          summing to 12.
 * https://oeis.org/A183663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*16^n + 30*15^n - 24*12^n - 40*10^n + 9*9^n + 16*8^n +
 *            14*7^n - 24*6^n + 10*5^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for calls with n < 1 */
        return 0;

    seq_t exp = (seq_t)n;
    seq_t result = 0;

    result += (seq_t)16 * big_pow((seq_t)16, exp);
    result += (seq_t)30 * big_pow((seq_t)15, exp);
    result -= (seq_t)24 * big_pow((seq_t)12, exp);
    result -= (seq_t)40 * big_pow((seq_t)10, exp);
    result += (seq_t) 9 * big_pow((seq_t) 9, exp);
    result += (seq_t)16 * big_pow((seq_t) 8, exp);
    result += (seq_t)14 * big_pow((seq_t) 7, exp);
    result -= (seq_t)24 * big_pow((seq_t) 6, exp);
    result += (seq_t)10 * big_pow((seq_t) 5, exp);

    return result;
}
