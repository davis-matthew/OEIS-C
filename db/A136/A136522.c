/*
 * A136522  a(n) = 1 if n is a palindrome, otherwise 0.
 * https://oeis.org/A136522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is a palindrome, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t rev = 0;
    while (tmp > 0) {
        rev = rev * 10 + tmp % 10;
        tmp /= 10;
    }
    return rev == n ? 1 : 0;
}
