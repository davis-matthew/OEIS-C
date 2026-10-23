/*
 * A040803  Continued fraction for sqrt(832).
 * https://oeis.org/A040803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction expansion of sqrt(832) a(0) = 28, for n > 0
 *            a(n) = period[(n-1)%12] where period =
 *            {1,5,2,2,1,13,1,2,2,5,1,56}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    static const seq_t period[12] = {
        (seq_t)1, (seq_t)5, (seq_t)2, (seq_t)2,
        (seq_t)1, (seq_t)13, (seq_t)1, (seq_t)2,
        (seq_t)2, (seq_t)5, (seq_t)1, (seq_t)56
    };
    return period[(n - 1) % 12];
}
