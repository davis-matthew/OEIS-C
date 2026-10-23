/*
 * A155181  a(n)=5*a(n-1)+a(n-2), n>2 ; a(0)=1, a(1)=4, a(2)=20 .
 * https://oeis.org/A155181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=5*a(n-1)+a(n-2), a(0)=1, a(1)=4, a(2)=20
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)20;

    seq_t prev2 = (seq_t)4;  /* a(1) */
    seq_t prev1 = (seq_t)20; /* a(2) */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = 5 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
