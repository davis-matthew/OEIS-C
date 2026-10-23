/*
 * A040475  Continued fraction for sqrt(498).
 * https://oeis.org/A040475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = floor(sqrt(498)) = 22; for n ≥ 1, a(n) = period[(n-1) mod
 *            6] with period = {3,6,22,6,3,44}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    static const seq_t period[6] = { (seq_t)3, (seq_t)6, (seq_t)22,
                                     (seq_t)6, (seq_t)3, (seq_t)44 };
    return period[(n - 1) % 6];
}
