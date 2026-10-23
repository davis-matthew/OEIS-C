/*
 * A168043  Let S(1)={1} and, for n>1 let S(n) be the smallest set containing
 *          x+1, x+2, and 2*x for each element x in S(n-1). a(n) is the number
 *          of elements in S(n).
 * https://oeis.org/A168043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168043: a(1)=1, a(2)=2, a(3)=4, a(n)=a(n-1)+a(n-2)+n-3 for n>3 */
seq_t a(idx_t n)
{
    /* Handle indices below the offset (just in case) */
    if (n < 1)
        return (seq_t)0;

    /* Small fixed cases */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)4;

    /* General recurrence: */
    /* prev2 = a(n-2), prev1 = a(n-1); start with a(2)=2 and a(3)=4 */
    seq_t prev2 = (seq_t)2;   /* a(2)   */
    seq_t prev1 = (seq_t)4;   /* a(3)   */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = prev1 + prev2 + (seq_t)(i - 3);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
