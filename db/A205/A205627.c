/*
 * A205627  Number of 3 X (n+1) 0..2 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+2*a(n-2)-28*a(n-3)+32*a(n-5) for n≥6 Initial
 *            terms: a(1)=72, a(2)=306, a(3)=1512, a(4)=8004, a(5)=43248
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* when called with an invalid index */
    if (n <= 5) {
        static const seq_t base[5] = { 72, 306, 1512, 8004, 43248 };
        return base[n - 1];
    }

    /* Sliding window of the last five computed values */
    seq_t t[5] = { 72, 306, 1512, 8004, 43248 };  /* t[4] = a(n-1) initially */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 6 * t[4] + 2 * t[3] - 28 * t[2] + 32 * t[0];
        /* shift the window one step to the right */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = next;
    }
    return t[4];   /* t[4] is a(n) after the loop */
}
