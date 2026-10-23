/*
 * A040642  Continued fraction for sqrt(668).
 * https://oeis.org/A040642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 25; for n>=1, a(n) = [1,5,2,12,2,5,1,50][(n-1)%8]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t periodic[8] = {1, 5, 2, 12, 2, 5, 1, 50};
    if (n == 0)
        return 25;
    return periodic[(n - 1) & 7];
}
