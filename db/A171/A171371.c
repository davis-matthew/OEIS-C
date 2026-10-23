/*
 * A171371  a(n) = 6*a(n-1) + 8*a(n-2) with a(1) = 8, a(2) = 18.
 * https://oeis.org/A171371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2), a(1) = 8, a(2) = 18
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A171371.  The function
   works for any build where seq_t is an integer type or an
   arbitrary-precision integer class. */
seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined – return 0. */
    if (n < 1) return 0;

    /* Handle the initial two values directly. */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)18;

    /* Iteratively compute the sequence up to a(n). */
    seq_t prevprev = (seq_t)8;   /* a(1) */
    seq_t prev     = (seq_t)18;   /* a(2) */
    seq_t cur;

    /* Compute a(3) ... a(n) using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)6 * prev + (seq_t)8 * prevprev;
        prevprev = prev;
        prev = cur;
    }

    /* At this point 'prev' holds a(n). */
    return prev;
}
