/*
 * A205085  a(n) = n 5's sandwiched between two 1's.
 * https://oeis.org/A205085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(0)=11, a(n)=10*a(n-1)+41
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 11;           /* a(0) */
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 41;  /* apply recurrence a(i+1) = 10*a(i)+41 */
    return res;
}
