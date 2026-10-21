/*
 * A055958  a(n) = n + reversal of base 9 digits of n (written in base 10).
 * https://oeis.org/A055958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n + reversal of base 9 digits of n
 * Range:     u8 n <= 83, u16 n <= 6964, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t rev = 0;
    idx_t temp = n;
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 9);
        rev = rev * 9 + digit;
        temp /= 9;
    }
    return n_val + rev;
}
