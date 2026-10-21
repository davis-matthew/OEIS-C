/*
 * A055960  n + reversal of base 11 digits of n (written in base 10).
 * https://oeis.org/A055960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reversal of base 11 digits of n (in decimal)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reverse the base‑11 representation of n */
    seq_t rev = 0;
    idx_t temp = n;
    while (temp > 0) {
        unsigned d = temp % 11;          /* digit in base 11 */
        rev = rev * 11 + d;              /* build reversed number */
        temp /= 11;
    }
    return (seq_t)n + rev;
}
