/*
 * A123224  a(n) = the first row sum of M^(n-1), where M = matrix(4,4,
 *          [1,1,1,1;0,1,2,3;0,1,3,6;0,1,4,10]).
 * https://oeis.org/A123224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 28*a(n-2) + 15*a(n-3) - a(n-4) with initial
 *            values a(1)=1, a(2)=4, a(3)=35, a(4)=428
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)35;
    if (n == 4) return (seq_t)428;

    /* a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)4;
    seq_t a3 = (seq_t)35;
    seq_t a4 = (seq_t)428;
    seq_t next = 0;

    /* Iterate from n=5 up to given n */
    for (idx_t i = 5; i <= n; ++i) {
        /* compute next = 15*a4 - 28*a3 + 15*a2 - a1
           rearranged to avoid intermediate negative values */
        seq_t sum = (seq_t)15 * a4 + (seq_t)15 * a2;
        seq_t sub = (seq_t)28 * a3 + a1;
        next = sum - sub;

        /* shift values for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
