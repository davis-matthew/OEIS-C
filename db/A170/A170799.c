/*
 * A170799  a(n) = n^10*(n^7 + 1)/2.
 * https://oeis.org/A170799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 14, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* n^10 */
    seq_t n10 = nn;
    for (unsigned int i = 1; i < 10; ++i) n10 *= nn;

    /* n^7 + 1 */
    seq_t n7 = nn;
    for (unsigned int i = 1; i < 7; ++i) n7 *= nn;
    n7 += (seq_t)1;

    /* divide the even factor first to avoid overflow */
    if ((n & (idx_t)1) == 0) {
        n10 = n10 / (seq_t)2;
    } else {
        n7 = n7 / (seq_t)2;
    }

    return n10 * n7;
}
