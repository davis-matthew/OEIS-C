/*
 * A040448  Continued fraction for sqrt(470).
 * https://oeis.org/A040448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction coefficients of sqrt(470); a(0) = 21; for n
 *            >= 1, a(n) = period[(n-1)%6] period = [1, 2, 8, 2, 1, 42]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)2, (seq_t)1, (seq_t)42
    };
    if (n == 0)
        return (seq_t)21;
    idx_t m = (n - 1) % 6;
    return period[m];
}
