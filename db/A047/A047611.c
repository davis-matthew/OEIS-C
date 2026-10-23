/*
 * A047611  Numbers that are congruent to {2, 4, 5} mod 8.
 * https://oeis.org/A047611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*k - 3 if n = 3*k a(n) = 8*k - 6 if n = 3*k - 2 a(n) =
 *            8*k - 4 if n = 3*k - 1 where k is a positive integer. This
 *            implements the OEIS A047611 (numbers congruent to 2, 4, or 5
 *            modulo 8) in O(1) time with no auxiliary state
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n modulo 3; n >= 1 as per offset */
    idx_t r = n % 3;

    if (r == 0) {                /* n = 3*k */
        seq_t k = (seq_t)(n / 3);
        return (seq_t)8 * k - (seq_t)3;
    } else if (r == 1) {         /* n = 3*k - 2  →  k = (n+2)/3 */
        seq_t k = (seq_t)((n + 2) / 3);
        return (seq_t)8 * k - (seq_t)6;
    } else {                     /* r == 2,  n = 3*k - 1  →  k = (n+1)/3 */
        seq_t k = (seq_t)((n + 1) / 3);
        return (seq_t)8 * k - (seq_t)4;
    }
}
