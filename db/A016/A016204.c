/*
 * A016204  Expansion of 1/((1-x)*(1-2*x)*(1-9*x)).
 * https://oeis.org/A016204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016204: Expansion of 1/((1-x)*(1-2*x)*(1-9*x))
   Recurrence: a(0)=1; a(n)=9*a(n-1)+2^(n+1)-1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t prev = (seq_t)1;          /* a(0) */
    seq_t pow2 = (seq_t)4;          /* 2^(1+1) for n=1 */
    seq_t cur;

    for (idx_t k = 1; k <= n; ++k) {
        cur = prev * (seq_t)9 + pow2 - (seq_t)1;
        pow2 = pow2 * (seq_t)2;    /* advance to 2^(k+2) */
        prev = cur;
    }
    return cur;
}
