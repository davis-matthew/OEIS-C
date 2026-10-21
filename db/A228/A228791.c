/*
 * A228791  Number of n X 3 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) for n>3, with a(1)=2, a(2)=8, a(3)=36
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)  /* undefined, return 0 for safety */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)8;
    if (n == 3)
        return (seq_t)36;

    /* iterative linear recurrence: a_n = 5*a_{n-1} - 3*a_{n-2} */
    seq_t prevprev = (seq_t)8;   /* a(2) */
    seq_t prev    = (seq_t)36;   /* a(3) */
    seq_t cur = prev;            /* initialize to a(3) */
    idx_t i;

    for (i = 4; i <= n; ++i) {
        cur = 5 * prev - 3 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
