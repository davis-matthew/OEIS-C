/*
 * A193867  Odd central polygonal numbers.
 * https://oeis.org/A193867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2-n+1 for even n; 2*n^2-3*n+2 for odd n
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    if ((n & 1ULL) == 0)          /* n even */
        return 2 * sq - nn + 1;
    else                          /* n odd  */
        return 2 * sq - 3 * nn + 2;
}
