/*
 * A143787  Number of compositions of n into floor((3*j)/2) kinds of j's for
 *          all j>=1.
 * https://oeis.org/A143787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c_n - c_{n-1} - c_{n-2} + c_{n-3} where (c_n) satisfies
 *            the linear recurrence c_n = 2*c_{n-1} + 3*c_{n-2} - c_{n-3} with
 *            c_0 = 1, c_1 = 2, c_2 = 7. This gives the OEIS sequence A143787:
 *            1,1,4,11,33,95,278,…
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th term of A143787 */
seq_t a(idx_t n)
{
    /* Handle the first three indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;   /* = c1 - c0 */
    if (n == 2) return (seq_t)4;   /* = c2 - c1 - c0 */

    /* c0, c1, c2 hold the three most recent c-values */
    seq_t c0 = (seq_t)1;  /* c_0 */
    seq_t c1 = (seq_t)2;  /* c_1 */
    seq_t c2 = (seq_t)7;  /* c_2 */
    seq_t a_n = 0;        /* placeholder for the term when i == n */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* c_i = 2*c_{i-1} + 3*c_{i-2} - c_{i-3} */
        seq_t c_next = (seq_t)2 * c2 + (seq_t)3 * c1 - c0;

        /* a_i = c_i - c_{i-1} - c_{i-2} + c_{i-3} */
        if (i == n)
            a_n = c_next - c2 - c1 + c0;

        /* Shift the window: c_{i-2} <- c_{i-1}, c_{i-1} <- c_i,
           c_i      <- c_next for next iteration */
        c0 = c1;
        c1 = c2;
        c2 = c_next;
    }

    return a_n;
}
