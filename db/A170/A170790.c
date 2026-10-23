/*
 * A170790  a(n) = n^9*(n^8 + 1)/2.
 * https://oeis.org/A170790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 * (n^8 + 1) / 2 (OEIS A170790)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 14, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                 /* Convert index to seq_t */
    seq_t n2 = sn * sn;                  /* n^2 */
    seq_t n4 = n2 * n2;                  /* n^4 */
    seq_t n8 = n4 * n4;                  /* n^8 */
    seq_t n9 = n8 * sn;                  /* n^9 */
    seq_t term = n8 + (seq_t)1;          /* n^8 + 1 */

    if ((n9 % (seq_t)2) == (seq_t)0) {
        /* n^9 is even – divide this factor by 2 first to avoid overflow */
        return (n9 / (seq_t)2) * term;
    } else {
        /* n^9 is odd – then n^8+1 must be even, divide that factor by 2 */
        return n9 * (term / (seq_t)2);
    }
}
