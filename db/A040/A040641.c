/*
 * A040641  Continued fraction for sqrt(667).
 * https://oeis.org/A040641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25; for n>=1 the terms repeat with period
 *            [1,4,1,3,7,8,2,8,7,3,1,4,1,50]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[14] = {
        1, 4, 1, 3, 7, 8, 2, 8, 7, 3, 1, 4, 1, 50
    };
    if (n == 0ULL)
        return 25;
    return period[(n - 1ULL) % 14ULL];
}
