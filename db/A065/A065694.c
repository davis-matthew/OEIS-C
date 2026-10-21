/*
 * A065694  Braided power sequence: A065692 is b(n+1) = 3*b(n) + 2*d(n) -
 *          c(n), A065693 is c(n+1) = 3*c(n) + 2*b(n) - d(n) and this is
 *          d(n+1) = 3*d(n) + 2*c(n) - b(n), starting with b(0) = 0, c(0) = 1
 *          and d(0) = 2.
 * https://oeis.org/A065694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 52*a(n-3) for n > 2, a(0) = 2,
 *            a(1) = 8, a(2) = 23. Implementation uses an iterative recurrence
 *            that works with any seq_t type (scalar or arbitrary‑precision)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)23;

    /* Sliding window of the last three terms: a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t t0 = (seq_t)2;   /* a_{i-3} */
    seq_t t1 = (seq_t)8;   /* a_{i-2} */
    seq_t t2 = (seq_t)23;  /* a_{i-1} */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)9 * t2 + (seq_t)52 * t0 - (seq_t)33 * t1;
        t0 = t1;
        t1 = t2;
        t2 = an;
    }
    return an;
}
