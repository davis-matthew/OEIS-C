/*
 * A189348  Number of n X 2 array permutations with each element moving one
 *          space diagonally, horizontally or vertically.
 * https://oeis.org/A189348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-3*a(n-3)-3*a(n-4)+a(n-5) for n≥6
 *            initial values: a(1)=1, a(2)=4, a(3)=13, a(4)=45, a(5)=160
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* values below offset are undefined */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)45;
    if (n == 5) return (seq_t)160;

    /* Sliding window of the last five values: a(i-5) ... a(i-1) */
    seq_t w[5];
    w[0] = (seq_t)1;   /* a1 */
    w[1] = (seq_t)4;   /* a2 */
    w[2] = (seq_t)13;  /* a3 */
    w[3] = (seq_t)45;  /* a4 */
    w[4] = (seq_t)160; /* a5 */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = w[4] * (seq_t)3
                   + w[3] * (seq_t)3
                   - w[2] * (seq_t)3
                   - w[1] * (seq_t)3
                   + w[0];
        /* shift window to make room for the new value */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = next;
    }
    return w[4];
}
