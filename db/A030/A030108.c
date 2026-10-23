/*
 * A030108  Base 9 reversal of n (written in base 10).
 * https://oeis.org/A030108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = reverse the base‑9 representation of n, then interpret
 *            the reversed digits as a base‑9 integer (yielding a decimal
 *            value)
 * Range:     u8 n <= 84, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t temp = n;
    while (temp > 0) {
        seq_t digit = temp % 9;
        ans = ans * 9 + digit;
        temp /= 9;
    }
    return ans;
}
