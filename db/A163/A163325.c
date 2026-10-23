/*
 * A163325  Pick digits at the even distance from the least significant end of
 *          the ternary expansion of n, then convert back to decimal.
 * https://oeis.org/A163325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (n mod 3) + 3*a(floor(n/9))
 * Range:     u8 n <= 728, u16 n <= 728, u32 n <= 728, u64 n <= 728,
 *            u128 n <= 728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t mult = 1;
    while (n > 0) {
        ans += mult * (seq_t)(n % 3);
        n /= 9;
        mult *= 3;
    }
    return ans;
}
