/*
 * A028859  a(n+2) = 2*a(n+1) + 2*a(n); a(0) = 1, a(1) = 3.
 * https://oeis.org/A028859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 2*a(n+1) + 2*a(n), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two seed values */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;

    /* Iteratively compute the sequence */
    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)3;   /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* next = 2*curr + 2*prev */
        seq_t next = curr;
        next += curr;        /* next = 2*curr */

        seq_t tmp = prev;
        tmp += tmp;          /* tmp = 2*prev */

        next += tmp;         /* next = 2*curr + 2*prev */

        prev = curr;         /* shift for next iteration */
        curr = next;
    }

    return curr;
}
