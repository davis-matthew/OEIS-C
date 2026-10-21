/*
 * A233942  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 4.
 * https://oeis.org/A233942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) - 13*a(n-3) + 2*a(n-4) for n >= 6
 *            Base values: a(1)=44, a(2)=226, a(3)=1150, a(4)=5874, a(5)=29970
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* outside the OEIS offset */

    /* Immediate return for the first five terms */
    if (n == 1) return (seq_t)44;
    if (n == 2) return (seq_t)226;
    if (n == 3) return (seq_t)1150;
    if (n == 4) return (seq_t)5874;
    if (n == 5) return (seq_t)29970;

    /* We will compute a(n) using the recurrence.
       Keep the last five values in a sliding window. */
    seq_t a1 = (seq_t)44;   /* a(n-4) */
    seq_t a2 = (seq_t)226;  /* a(n-3) */
    seq_t a3 = (seq_t)1150; /* a(n-2) */
    seq_t a4 = (seq_t)5874; /* a(n-1) */
    seq_t a5 = (seq_t)29970;/* a(n  ) */
    seq_t an = a5;          /* will hold the current value */

    for (idx_t k = 6; k <= n; ++k) {
        an = (seq_t)5 * a5 + (seq_t)3 * a4 + (seq_t)2 * a2
             - (seq_t)13 * a3;
        /* Shift the window forward */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
