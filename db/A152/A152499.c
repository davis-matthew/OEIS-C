/*
 * A152499  1/12 of the number of permutations of 3 indistinguishable copies
 *          of 1..n with exactly 2 local maxima.
 * https://oeis.org/A152499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 176*a(n-2) + 320*a(n-3) (n > 3) initial
 *            values: a(1) = 0, a(2) = 1, a(3) = 30
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)30;

    /* Iteratively apply the recurrence. */
    seq_t a1 = (seq_t)0;   /* a(i-3)   */
    seq_t a2 = (seq_t)1;   /* a(i-2)   */
    seq_t a3 = (seq_t)30;  /* a(i-1)   */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = ((seq_t)28 * a3)              /* 28*a(i-1) */
            - ((seq_t)176 * a2)             /* 176*a(i-2) */
            + ((seq_t)320 * a1);            /* 320*a(i-3) */

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
