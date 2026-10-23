/*
 * A192374  Coefficient of x in the reduction of the polynomial p(n,x) defined
 *          at A162517 and below in Comments.
 * https://oeis.org/A192374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A192374
 * Polynomial reduction coefficient.
 * Recurrence (valid for n >= 5):
 *   a(n) = 2*a(n-1) + 10*a(n-2) - 6*a(n-3) - 9*a(n-4)
 * Initial values:
 *   a(1) = 0, a(2) = 2, a(3) = 4, a(4) = 28
 */
seq_t a(idx_t n)
{
    /* Handle minimal offset and trivial cases */
    if (n == 0)          return 0;
    if (n == 1)          return 0;
    if (n == 2)          return 2;
    if (n == 3)          return 4;
    if (n == 4)          return 28;

    /* Sliding window for the recurrence */
    seq_t a1 = 0;   /* a(n-4) */
    seq_t a2 = 2;   /* a(n-3) */
    seq_t a3 = 4;   /* a(n-2) */
    seq_t a4 = 28;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t tmp = (seq_t)2 * a4;
        tmp += (seq_t)10 * a3;
        tmp -= (seq_t)6 * a2;
        tmp -= (seq_t)9 * a1;

        /* Shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = tmp;
    }

    return a4;   /* a(n) after the loop */
}
