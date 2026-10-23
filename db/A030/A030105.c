/*
 * A030105  Base 6 reversal of n (written in base 10).
 * https://oeis.org/A030105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = reverse the base‑6 digits of n and interpret the reversed
 *            string as a base‑6 number. Equivalently, if n = Σ_{i=0}^{k}
 *            d_i·6^i (0 ≤ d_i < 6), then a(n) = Σ_{i=0}^{k} d_i·6^{k-i}
 * Range:     u8 n <= 217, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t temp = n;
    while (temp)
    {
        /* Extract least significant base‑6 digit. */
        seq_t digit = (seq_t)(temp % 6);
        /* Append this digit as the next most significant digit of res. */
        res = res * 6 + digit;
        temp /= 6;
    }
    return res;
}
