/*
 * A093103  a(n+2) = 8*a(n+1) + 21*a(n), with a(1)=1, a(2)=8.
 * https://oeis.org/A093103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 21*a(n-2) with a(1)=1, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1 – return 0 as a harmless default */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)8;   /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)8 * prev1 + (seq_t)21 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
