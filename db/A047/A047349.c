/*
 * A047349  Numbers that are congruent to {0, 2, 4} mod 7.
 * https://oeis.org/A047349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/3) + 2 * ((n-1) % 3)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers congruent to {0, 2, 4} mod 7 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – defined as 0 */
    idx_t m = n - 1;               /* shift to 0‑based index   */
    idx_t q = m / 3;               /* floor((n-1)/3)           */
    idx_t r = m % 3;               /* (n-1) mod 3              */
    return (seq_t)q * 7 + (seq_t)r * 2;
}
