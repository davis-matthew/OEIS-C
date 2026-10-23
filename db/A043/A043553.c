/*
 * A043553  Row sums of convolution triangle A030524.
 * https://oeis.org/A043553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 30*a(n-2) + 30*a(n-3),
   with a(1) = 1, a(2) = 7, a(3) = 43 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)43;

    /* Iterate using the recurrence for n ≥ 4 */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)7;   /* a(2) */
    seq_t a3 = (seq_t)43;  /* a(3) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i)
    {
        ai = (seq_t)10 * a3 - (seq_t)30 * a2 + (seq_t)30 * a1;
        /* Slide the window */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
