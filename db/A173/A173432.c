/*
 * A173432  NW-SE diagonal sums of Riordan array A112468.
 * https://oeis.org/A173432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats the pattern 1,1,2,1,1,0 with period 6
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, return 0 */
    static const seq_t period[6] = {1, 1, 2, 1, 1, 0};
    return period[(n - 1) % 6];
}
