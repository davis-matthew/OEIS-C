/*
 * A163090  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 14 a(2) = 182 a(3) = 2366 a(4) = 30667 a(n) =
 *            12*a(n-1) + 12*a(n-2) + 12*a(n-3) - 78*a(n-4) for n >= 5
 *            (derived from the generating function (t^4 + 2*t^3 + 2*t^2 + 2*t
 *            + 1)/(78*t^4 - 12*t^3 - 12*t^2 - 12*t + 1) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)182;
    if (n == 3) return (seq_t)2366;
    if (n == 4) return (seq_t)30667;

    /* Use a sliding window of the last four values */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)14;         /* a(n-3) */
    seq_t a2 = (seq_t)182;        /* a(n-2) */
    seq_t a3 = (seq_t)2366;       /* a(n-1) */
    seq_t a4 = (seq_t)30667;      /* a(n)  for n=4 */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* a(i) = 12*a(i-1) + 12*a(i-2) + 12*a(i-3) - 78*a(i-4) */
        seq_t term1 = 12 * a4;
        seq_t term2 = 12 * a3;
        seq_t term3 = 12 * a2;
        seq_t term4 = 78 * a1;
        seq_t an = term1 + term2 + term3 - term4;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
