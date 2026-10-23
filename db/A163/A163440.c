/*
 * A163440  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1)+13*a(n-2)+13*a(n-3)+13*a(n-4)-91*a(n-5) for n
 *            >= 6; a(0)=1, a(1)=15, a(2)=210, a(3)=2940, a(4)=41160,
 *            a(5)=576135
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first six terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)210;
    if (n == 3) return (seq_t)2940;
    if (n == 4) return (seq_t)41160;
    if (n == 5) return (seq_t)576135;

    /* initialise the last five values */
    seq_t a0 = (seq_t)1;          /* a(n-5) */
    seq_t a1 = (seq_t)15;         /* a(n-4) */
    seq_t a2 = (seq_t)210;        /* a(n-3) */
    seq_t a3 = (seq_t)2940;       /* a(n-2) */
    seq_t a4 = (seq_t)41160;      /* a(n-1) */
    seq_t val;

    /* compute a(5) using the inhomogeneous term +1 */
    val = (seq_t)13 * a4 + (seq_t)13 * a3 + (seq_t)13 * a2 + (seq_t)13 * a1
          - (seq_t)91 * a0 + (seq_t)1;
    /* shift the window for subsequent indices */
    a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = val;

    /* now compute a(k) for k = 6 .. n inclusively */
    for (idx_t k = 6; k <= n; ++k) {
        val = (seq_t)13 * a4 + (seq_t)13 * a3 + (seq_t)13 * a2 + (seq_t)13 * a1
              - (seq_t)91 * a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = val;
    }
    return a4;
}
