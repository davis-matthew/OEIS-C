/*
 * A200039  Number of -n..n arrays x(0..2) of 3 elements with sum zero and
 *          with zeroth through 2nd differences all nonzero.
 * https://oeis.org/A200039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2 - 5*n + (2 if n odd) ; a(1) = 0 (OEIS offset 1)
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS definition gives a(1)=0, a(2)=2, a(3)=14, … */
    if (n <= 1U)
        return (seq_t)0;               /* a(0) and a(1) – offset handling */

    seq_t tn  = (seq_t)n;               /* convert index to seq_t */
    seq_t res = tn * ( (seq_t)3 * tn - (seq_t)5 );

    if (n & 1U)                         /* add the +2 term for odd n */
        res += (seq_t)2;
    return res;
}
