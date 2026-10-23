/*
 * A040565  Continued fraction for sqrt(590).
 * https://oeis.org/A040565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = a(n-6), with a(0)=24, a(6k)=48 for k>0, and the cycle [3,
 *            2, 4, 2, 3] for n%6 = 1..5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pattern for n > 0 */
    static const seq_t pattern[6] = {48, 3, 2, 4, 2, 3};
    if (n == 0)
        return 24;
    return pattern[n % 6];
}
