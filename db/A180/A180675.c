/*
 * A180675  The Ca3 sums of the Pell-Jacobsthal triangle A013609.
 * https://oeis.org/A180675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*a(n-1) - 3*a(n-2) + a(n-3) (a(0)=1, a(1)=1, a(2)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A180675 by a simple linear recurrence.
   The computation is performed iteratively and does not depend on
   previous calls, so calls may be made in any order. */
seq_t a(idx_t n) {
    /* small n handled immediately */
    if (n <= 2)
        return (seq_t)1;

    /* recurrence variables: a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* next = 11*a2 - 3*a1 + a0 */
        next = (a2 * (seq_t)11) - (a1 * (seq_t)3) + a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
