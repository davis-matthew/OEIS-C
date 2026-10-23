/*
 * A040276  Continued fraction for sqrt(294).
 * https://oeis.org/A040276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 17; for n >= 1, a(n) = period[(n-1) mod 6] where period =
 *            {6, 1, 4, 1, 6, 34}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)17;
    static const seq_t period[6] = {(seq_t)6, (seq_t)1, (seq_t)4,
                                    (seq_t)1, (seq_t)6, (seq_t)34};
    return period[(n - 1) % 6];
}
