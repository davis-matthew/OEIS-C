/*
 * A010214  Continued fraction for sqrt(166).
 * https://oeis.org/A010214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(166) (periodic). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 12;
    static const seq_t period[22] = {
        1, 7, 1, 1, 1, 2, 4, 1, 3, 2,
        12, 2, 3, 1, 4, 2, 1, 1, 1, 7,
        1, 24
    };
    idx_t idx = (n - 1) % 22;
    return period[idx];
}
