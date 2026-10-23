/*
 * A198845  a(n) = 8*6^n - 1.
 * https://oeis.org/A198845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*6^n - 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 6;
    idx_t exp    = n;

    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }

    result *= 8;
    result -= 1;
    return result;
}
