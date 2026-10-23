/*
 * A129973  a(n) = A000045(n) - A000931(n).
 * https://oeis.org/A129973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-4) - a(n-5) Initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial and offset cases */
    if (n == 0) return 0;  /* undefined for offset 1, return 0 */

    /* Pre‑computed initial values */
    const seq_t init[5] = { 0, 0, 0, 1, 1 };

    if (n <= 5)
        return init[n - 1];

    /* Variables hold the last five values:
       prev5 = a(n-5), prev4 = a(n-4), prev3 = a(n-3),
       prev2 = a(n-2), prev1 = a(n-1) */
    seq_t prev5 = init[0];
    seq_t prev4 = init[1];
    seq_t prev3 = init[2];
    seq_t prev2 = init[3];
    seq_t prev1 = init[4];
    seq_t curr;

    /* Iterate from n = 6 up to the requested n */
    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 2*a(i-2) - 2*a(i-4) - a(i-5) */
        curr = prev1 + (prev2 + prev2) - ((prev4 + prev4) + prev5);

        /* Shift the window of the last five values */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr; /* prev1 now equals a(n) */
}
