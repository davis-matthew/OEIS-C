/*
 * A040779  Continued fraction for sqrt(808).
 * https://oeis.org/A040779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 28; for n>0, a(n) = p[(n-1) mod 18] where * the period p
 *            of sqrt(808) is * {2, 2, 1, 5, 1, 1, 1, 1, 13, 1, 1, 1, 1, 5, 1,
 *            2, 2, 56}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t period[18] = {
    2,  2,  1,  5,  1,  1,  1,  1,
   13,  1,  1,  1,  1,  5,  1,  2,  2, 56
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    return period[(int)((n - 1) % 18)];
}
