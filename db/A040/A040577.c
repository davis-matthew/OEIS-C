/*
 * A040577  Continued fraction for sqrt(602).
 * https://oeis.org/A040577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction terms for sqrt(602). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;
    static const seq_t period[6] = {1, 1, 6, 1, 1, 48};
    return period[(n - 1) % 6];
}
