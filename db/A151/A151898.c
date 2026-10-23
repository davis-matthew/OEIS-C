/*
 * A151898  First differences of Frobenius numbers for 7 successive numbers
 *          A138987.
 * https://oeis.org/A151898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/6 + 1 if n mod 6 ≠ 0 a(n) = 7 * (n/6) + 2 if n mod 6 =
 *            0 (where n ≥ 1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 6;          /* floor division */
    if (n % 6 == 0) {
        /* use seq_t for the result */
        return (seq_t)(7 * m + 2);
    } else {
        return (seq_t)(m + 1);
    }
}
