/*
 * A109094  Length of the longest path (repeated edges not allowed) between
 *          two arbitrary, distinct nodes in K_n, the complete graph on n
 *          vertices.
 * https://oeis.org/A109094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n > 1: if n is odd, a(n) = n*(n-1)/2 - 1 if n is
 *            even, a(n) = n*(n-2)/2 + 1 This uses the closed‑form derived
 *            from the OEIS definition
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* cast once to seq_t to avoid sign problems with unsigned types */
    seq_t sn = (seq_t)n;

    if (n & 1u)   /* odd */
    {
        seq_t tmp = sn * (sn - 1u);
        tmp /= 2;
        tmp -= 1;
        return tmp;
    }
    else          /* even */
    {
        seq_t tmp = sn * (sn - 2u);
        tmp /= 2;
        tmp += 1;
        return tmp;
    }
}
