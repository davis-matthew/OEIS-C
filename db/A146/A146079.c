/*
 * A146079  Period 9: repeat 2,4,8,5,4,5,8,4,2.
 * https://oeis.org/A146079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 2) mod 9, which equals the repeating pattern
 *            2,4,8,5,4,5,8,4,2 with period 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 first to avoid overflows in the calculation. */
    idx_t m = n % 9;                     /* 0 <= m < 9 */

    /* Compute (m * (m + 1)) mod 9. The intermediate product fits in
       seq_t easily because 0 <= m <= 8. */
    seq_t prod = (seq_t)m * (seq_t)((m + 1) % 9);

    /* Finally add 2 and take modulo 9. */
    return (prod + 2) % 9;
}
