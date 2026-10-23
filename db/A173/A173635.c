/*
 * A173635  Period 5: repeat [1, 1, 4, 4, 2].
 * https://oeis.org/A173635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + floor((n mod 5)/2))^2 mod 7
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n % 5;           /* n mod 5 */
    seq_t t = (seq_t)(1 + (k / 2));  /* 1 + floor(k/2) */
    t = t * t;                 /* square */
    t = t % (seq_t)7;          /* modulo 7 */
    return t;
}
