/*
 * A048878  Generalized Pellian with second term of 9.
 * https://oeis.org/A048878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) + a(n-2); a(0)=1, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0)
        return 1;
    if (n == 1)
        return 9;

    /* a0 = a(n-2), a1 = a(n-1).  Start with a0=1, a1=9. */
    seq_t a0 = 1;
    seq_t a1 = 9;
    seq_t an;                          /* will hold a(n) during the loop */

    /* Compute the terms one by one until the requested index. */
    for (idx_t i = 2; i <= n; ++i) {
        an = 4 * a1 + a0;      /* a(i) = 4*a(i-1) + a(i-2) */
        a0 = a1;               /* shift window: a(i-2) <- a(i-1) */
        a1 = an;               /* shift window: a(i-1) <- a(i)   */
    }

    return an;   /* finally, a(n) */
}
