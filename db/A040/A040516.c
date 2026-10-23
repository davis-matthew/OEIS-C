/*
 * A040516  Continued fraction for sqrt(540).
 * https://oeis.org/A040516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23, period=[4,4,1,10,1,4,4,46]; a(n)=period[(n-1) mod 8]
 *            for n>=1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 23;
    static const seq_t period[8] = {4, 4, 1, 10, 1, 4, 4, 46};
    return period[(n - 1) & 7];
}
