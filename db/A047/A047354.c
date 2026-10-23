/*
 * A047354  Numbers that are congruent to {0, 1, 2} mod 7.
 * https://oeis.org/A047354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + ((n-1) mod 3) (offset 1)
 * Range:     u8 n <= 111, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with n < 1, return 0 (undefined for the sequence) */
    if (n == 0) return 0;

    idx_t m = n - 1;                    /* adjust for offset 1 */
    seq_t q = (seq_t)(m / 3);           /* floor((n-1)/3) */
    seq_t r = (seq_t)(m % 3);           /* (n-1) mod 3 */

    return q * 7 + r;
}
