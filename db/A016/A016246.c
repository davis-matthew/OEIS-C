/*
 * A016246  Expansion of 1/((1-x)(1-6x)(1-10x)).
 * https://oeis.org/A016246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=17, a(n)=16*a(n-1)-60*a(n-2)+1 for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)17;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)17;  /* a(n-1) */
    seq_t curr   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)16 * prev1 - (seq_t)60 * prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
