/*
 * A163527  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1)+25*a(n-2)+25*a(n-3)+25*a(n-4)-325*a(n-5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(5) = 25*a(4)+25*a(3)+25*a(2)+25*a(1)-325*a(0)+1         */
/* Initial values: a(0)=1, a(1)=27, a(2)=702, a(3)=18252, a(4)=474552 */

seq_t a(idx_t n)
{
    /* Return the known initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)702;
    if (n == 3) return (seq_t)18252;
    if (n == 4) return (seq_t)474552;

    /* Sliding window for the last five terms. */
    seq_t a0 = (seq_t)1,     /* a(n-5)   */
          a1 = (seq_t)27,    /* a(n-4)   */
          a2 = (seq_t)702,   /* a(n-3)   */
          a3 = (seq_t)18252, /* a(n-2)   */
          a4 = (seq_t)474552;/* a(n-1)   */

    seq_t an = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute the next term using the recurrence. */
        if (i == 5) {
            /* The inhomogeneous term from the numerator is +1 only for n=5. */
            an = ((seq_t)25) * (a4 + a3 + a2 + a1) - ((seq_t)325) * a0 + (seq_t)1;
        } else {
            an = ((seq_t)25) * (a4 + a3 + a2 + a1) - ((seq_t)325) * a0;
        }

        /* Shift the sliding window. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
