/*
 * A030267  Compose the natural numbers with themselves, A(x) = B(B(x)) where
 *          B(x) = x/(1-x)^2 is the generating function for natural numbers.
 * https://oeis.org/A030267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) Initial values
 *            (offset 1): a(1)=1, a(2)=4, a(3)=14, a(4)=46
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first few terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)46;

    /* a1..a4 hold the last four computed values */
    seq_t a1 = (seq_t)1;   /* a(n-4)   */
    seq_t a2 = (seq_t)4;   /* a(n-3)   */
    seq_t a3 = (seq_t)14;  /* a(n-2)   */
    seq_t a4 = (seq_t)46;  /* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t an = 6 * a4 - 11 * a3 + 6 * a2 - a1;
        /* shift the window forwards */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;   /* a(n) */
}
