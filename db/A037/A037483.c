/*
 * A037483  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2) - 6*a(n-3) Initial values: a(1) = 1,
 *            a(2) = 8, a(3) = 49
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)49;

    /* Use the recurrence to compute subsequent terms.
       a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)8;   /* a(2) */
    seq_t a3 = (seq_t)49;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * (seq_t)6 + a2;
        next -= a1 * (seq_t)6;
        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
