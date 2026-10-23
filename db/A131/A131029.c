/*
 * A131029  Periodic sequence (11, 5, 2, 5, 11, 14).
 * https://oeis.org/A131029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) with period 6 a(1) = 11, a(2) = 5, a(3) = 2, a(4)
 *            = 5, a(5) = 11, a(6) = 14, for n>6, a(n)=a(n-6)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = { 11, 5, 2, 5, 11, 14 };
    idx_t r = (n - 1) % 6;          /* n >= 1 guarantees n-1 fits in idx_t */
    return period[r];
}
