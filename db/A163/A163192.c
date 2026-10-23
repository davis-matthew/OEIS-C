/*
 * A163192  a(n) = 20*a(n-1)-93*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1)-93*a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* iterative computation */
    seq_t prev = (seq_t)1;   /* a(n-2) */
    seq_t curr = (seq_t)10;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)20 * curr - (seq_t)93 * prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
