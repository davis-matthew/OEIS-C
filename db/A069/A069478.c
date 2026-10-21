/*
 * A069478  First differences of A069477, successive differences of (n+1)^5 -
 *          n^5.
 * https://oeis.org/A069478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120*(n+2)
 * Range:     u8 none, u16 n <= 544, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 (outside the offset)
       we return 0, although this value is never used. */
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;            /* convert index to the seq_t type */
    return 120 * (nn + 2);          /* 120·(n+2) gives a(1)=360, a(2)=480, … */
}
