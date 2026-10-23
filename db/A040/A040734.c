/*
 * A040734  Continued fraction for sqrt(762).
 * https://oeis.org/A040734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 27; for n ≥ 1 the continued fraction terms of sqrt(762)
 *            repeat with period 10: [1,1,1,1,8,1,1,1,1,54]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[10] = {1,1,1,1,8,1,1,1,1,54};
    if (n == 0)
        return 27;
    return period[(n - 1) % 10];
}
