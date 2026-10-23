/*
 * A101368  The sequence solves the following problem: find all the pairs
 *          (i,j) such that i divides 1+j+j^2 and j divides 1+i+i^2. In fact,
 *          the pairs (a(n),a(n+1)), n>0, are all the solutions.
 * https://oeis.org/A101368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101368: a(1)=1, a(2)=1, a(3)=3
   a(n) = 6*a(n-1) - 6*a(n-2) + a(n-3)   for n > 3
   (equivalently a(n+1) = (1 + a(n) + a(n)^2)/a(n-1))
*/
seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)1;
    }
    if (n == 3) {
        return (seq_t)3;
    }

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;      /* a(1) */
    seq_t a1 = (seq_t)1;      /* a(2) */
    seq_t a2 = (seq_t)3;      /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 6 * a2 - 6 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
