/*
 * A028012  Expansion of 1/((1-2x)(1-7x)(1-10x)(1-11x)).
 * https://oeis.org/A028012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 313*a(n-2) + 1284*a(n-3) - 1540*a(n-4), a(0)
 *            = 1, a(1) = 30, a(2) = 587, a(3) = 9504
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)587;
    if (n == 3) return (seq_t)9504;

    /* Initialize the four previous values. */
    seq_t a0 = (seq_t)1;          /* a(n-4)   */
    seq_t a1 = (seq_t)30;         /* a(n-3)   */
    seq_t a2 = (seq_t)587;        /* a(n-2)   */
    seq_t a3 = (seq_t)9504;       /* a(n-1)   */

    /* Compute successive terms until n. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* 30*a3 + 1284*a1  (positive terms)  */
        seq_t plus  = (seq_t)30 * a3 + (seq_t)1284 * a1;
        /* 313*a2 + 1540*a0 (negative terms) */
        seq_t minus = (seq_t)313 * a2 + (seq_t)1540 * a0;
        seq_t next  = plus - minus;      /* a(n) */

        /* Shift for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
