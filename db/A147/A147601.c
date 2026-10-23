/*
 * A147601  First differences of A132355.
 * https://oeis.org/A147601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9 + 5*(-1)^n)*(n+1))/2 == 7*(n+1) if n is even, 2*(n+1)
 *            if n is odd
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n+1 is computed in idx_t to avoid overflow, then cast to seq_t. */
    seq_t term = (seq_t)(n + 1);
    if ((n & 1ULL) == 0)          /* n is even */
        return term * (seq_t)7;
    else                          /* n is odd */
        return term * (seq_t)2;
}
