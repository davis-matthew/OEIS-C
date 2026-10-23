/*
 * A047535  Numbers that are congruent to {4, 7} mod 8.
 * https://oeis.org/A047535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (1 + (-1)^n)/2 (equivalently a(n) = 4*n -
 *            (n%2==0?1:0) )
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert index to seq_t for multiplication */
    seq_t val  = (seq_t)4 * nseq;   /* 4 * n */

    /* if n is even subtract 1; if odd subtract 0 */
    if ((n & 1ULL) == 0)
        val -= 1;

    return val;
}
