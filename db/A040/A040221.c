/*
 * A040221  Continued fraction for sqrt(237).
 * https://oeis.org/A040221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 15; for n >= 1, a(n) = periodic[(n-1) % 10] where
 *            periodic = {2,1,1,7,10,7,1,1,2,30}
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t periodic[10] = {2, 1, 1, 7, 10, 7, 1, 1, 2, 30};
    if (n == 0)
        return (seq_t)15;
    return periodic[(n - 1) % 10];
}
