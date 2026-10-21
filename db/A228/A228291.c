/*
 * A228291  a(n) = Sum_{k=1..7} n^k.
 * https://oeis.org/A228291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^1 + n^2 + n^3 + n^4 + n^5 + n^6 + n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t pow   = base;  /* n^1 */
    seq_t sum   = 0;

    for (int k = 1; k <= 7; ++k) {
        sum += pow;      /* add n^k */
        pow *= base;    /* next power */
    }
    return sum;
}
