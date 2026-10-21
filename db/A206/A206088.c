/*
 * A206088  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 4*a(n-3) - 10*a(n-4) for n>6 Initial
 *            values: a(1)=270, a(2)=780, a(3)=2016, a(4)=5952, a(5)=17976,
 *            a(6)=54432. The function recomputes the sequence from scratch;
 *            no dynamic allocation is required
 * Range:     u8 none, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) {
        return 0; /* undefined for indices below 1 */
    }
    /* Base cases */
    if (n == 1) return 270;
    if (n == 2) return 780;
    if (n == 3) return 2016;
    if (n == 4) return 5952;
    if (n == 5) return 17976;
    if (n == 6) return 54432;

    /* Iterate from 7 up to n using a sliding window of the last four
       values.  The window holds a(i-4), a(i-3), a(i-2), a(i-1). */
    seq_t v0 = 2016;  /* a3  */
    seq_t v1 = 5952;  /* a4  */
    seq_t v2 = 17976; /* a5  */
    seq_t v3 = 54432; /* a6  */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = a(i-1) + 6*a(i-2) + 4*a(i-3) - 10*a(i-4) */
        seq_t cur = v3 + 6 * v2 + 4 * v1 - 10 * v0;

        /* Slide the window forward */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = cur;
    }

    return v3; /* v3 now holds a(n) */
}
