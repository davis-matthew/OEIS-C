/*
 * A026583  a(n) = Sum{T(i,j)}, 0<=j<=i, 0<=i<=2n, T given by A026568.
 * https://oeis.org/A026583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(0) = 1, a(1) = 4, a(2)
 *            = 11, * a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) for n >= 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The sequence is defined by the generating function
 *        (1+2x)/[(1-x)(1-x-4x^2)]  and the recurrence follows
 *        from the denominator of that function. */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;

    /* iterate the recurrence up to the requested index */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)4;   /* a(1)   */
    seq_t a2 = (seq_t)11;  /* a(2)   */
    seq_t new_val;

    for (idx_t i = 3; i <= n; ++i) {
        /* new_val = 2*a2 + 3*a1 - 4*a0   (all intermediate values
         * fit in seq_t for values that fit in seq_t) */
        new_val = (seq_t)2 * a2;
        new_val += (seq_t)3 * a1;
        new_val -= (seq_t)4 * a0;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }
    return a2;
}
