/*
 * A037314  Numbers whose base-3 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the base-3 digits of n as base-9 digits
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow9 = 1;           /* 9^0 */
    while (n) {
        idx_t digit = n % 3;
        if (digit)
            res += pow9 * (seq_t)digit;
        n /= 3;
        pow9 *= 9;
    }
    return res;
}
