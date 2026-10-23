/*
 * A040222  Continued fraction for sqrt(238).
 * https://oeis.org/A040222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction coefficients for sqrt(238).
   a(0) = 15, and for n > 0
   a(n) = [2,2,1,14,1,2,2,30][(n-1) mod 8] */
seq_t a(idx_t n)
{
    static const seq_t period[8] = {2, 2, 1, 14, 1, 2, 2, 30};
    if (n == 0)
        return (seq_t)15;
    return period[(n - 1) & 7];
}
