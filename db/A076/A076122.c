/*
 * A076122  a(1)=1, a(2)=2 and then n^2 times a(n) is the product of its
 *          neighbors.
 * https://oeis.org/A076122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = (n-1)^2 * a(n-1) / a(n-2) with a(1)=1, a(2)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)2;
    }

    /* Use two variables to hold consecutive terms. */
    seq_t prevprev = (seq_t)1;   /* a(n-2) */
    seq_t prev     = (seq_t)2;   /* a(n-1) */
    seq_t current  = (seq_t)0;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t im1 = (seq_t)(i - 1);              /* i - 1 as seq_t */
        seq_t num = im1 * im1 * prev;             /* (n-1)^2 * a(n-1) */
        current = num / prevprev;                /* integer division */
        prevprev = prev;                         /* shift window */
        prev = current;
    }
    return prev; /* after loop prev == a(n) for n >= 3 */
}
