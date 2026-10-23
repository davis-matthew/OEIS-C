/*
 * A014401  Denominators of coefficients of odd powers of x of the expansion
 *          of Bessel function J_3(x).
 * https://oeis.org/A014401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+3) * n! * (n+3)!. Recurrence: a(n) =
 *            4*n*(n+3)*a(n-1), a(0) = 48
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)48;

    seq_t result = (seq_t)48;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)4 * (seq_t)i * (seq_t)(i + 3);
        result *= mult;
    }
    return result;
}
