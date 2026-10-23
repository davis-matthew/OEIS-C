/*
 * A047249  Numbers that are congruent to {3, 4, 5} mod 6.
 * https://oeis.org/A047249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*floor((n-1)/3) + 3 + (n-1)%3
 * a(1)=3, a(2)=4, a(3)=5
 */
seq_t a(idx_t n)
{
    /* If called with n = 0 (outside the offset) return 0 */
    if (n == 0) {
        return 0;
    }

    /* m = n-1 is safe because n>=1 */
    idx_t m = n - 1;

    /* Compute quotient and remainder of m divided by 3 */
    seq_t q = (seq_t)(m / 3);
    seq_t r = (seq_t)(m % 3);

    /* Return the corresponding term */
    return 6 * q + 3 + r;
}
