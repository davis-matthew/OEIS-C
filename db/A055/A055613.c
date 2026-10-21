/*
 * A055613  n!*LaguerreL(n,4,-8).
 * https://oeis.org/A055613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * LaguerreL(n,4,-8) Recurrence: a(0)=1, a(1)=13,
 *            a(n+1) = (2n+13) * a(n) - n*(n+4) * a(n-1) (for n>=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)13;  /* a(1) */
    seq_t curr  = 0;

    /* iterate from 1 up to n-1; at iteration i we compute a(i+1) */
    idx_t i;
    for (i = 1; i < n; ++i) {
        /* term1 = (2i + 13) * a(i)  */
        seq_t term1 = ( (seq_t)( (idx_t)2 * i + (idx_t)13 ) ) * prev1;
        /* term2 = i*(i+4) * a(i-1) */
        seq_t term2 = ( (seq_t)i ) * ( (seq_t)(i + (idx_t)4) ) * prev2;
        curr = term1 - term2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
