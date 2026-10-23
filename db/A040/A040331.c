/*
 * A040331  Continued fraction for sqrt(350).
 * https://oeis.org/A040331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 18, a(n) = 1,2,2,2,1,36 repeating for n ≥ 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;
    static const seq_t period[6] = {1, 2, 2, 2, 1, 36};
    return period[(n - 1) % 6];
}
