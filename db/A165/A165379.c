/*
 * A165379  Number of slanted nX6 (i=1..n)X(j=i..6+i-1) 1..4 arrays with all
 *          1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 2 neighbors w...
 * https://oeis.org/A165379
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=101, a(3)=233, a(4)=265, a(n)=74*n-35 for n>=5
 *            (equivalently a(n)=2*a(n-1)-a(n-2) for n>=7)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the exceptional small indices explicitly. */
    if (n == 2u) return (seq_t)101u;
    if (n == 3u) return (seq_t)233u;
    if (n == 4u) return (seq_t)265u;

    /* For n >= 5 use the linear formula.  All operations are done
       in seq_t to avoid overflows of intermediate products. */
    seq_t res = (seq_t)n;
    res = res * (seq_t)74u;
    res = res - (seq_t)35u;
    return res;
}
