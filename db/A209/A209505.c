/*
 * A209505  Half the number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          having two or four distinct clockwise edge differences.
 * https://oeis.org/A209505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2), a(1) = 27, a(2) = 123
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A209505. */

seq_t a(idx_t n)
{
    /* Return the first two terms directly. */
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)123;

    /* Previous two terms */
    seq_t a_prev = (seq_t)27;   /* a(1) */
    seq_t a_curr = (seq_t)123;  /* a(2) */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        a_next = (seq_t)5 * a_curr - (seq_t)2 * a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
