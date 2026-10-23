/*
 * A144621  a(0) = a(1) = 1 and a(n+2) = a(n+1) * (3*a(n+1) - 2*a(n)^2), n>=0.
 * https://oeis.org/A144621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3,
   a(n) = a(n-1) * (3*a(n-1) - 2 * a(n-2)^2) for n >= 3 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)3;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = prev1 * ((seq_t)3 * prev1 - (seq_t)2 * prev2 * prev2);
        prev2 = prev1;
        prev1 = temp;
    }
    return prev1;
}
