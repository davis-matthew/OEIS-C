/*
 * A224459  The Wiener index of the straight pentachain of n pentagonal rings
 *          (see Fig. 2.1 in the A. A. Ali et al. reference).
 * https://oeis.org/A224459
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^3 + 21*n^2 - 6*n + 14)/2
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined – return 0 safely. */
    if (n < 2)
        return (seq_t)0;

    seq_t nn = (seq_t)n;        /* n as seq_t */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */

    seq_t term1 = (seq_t)3 * n3;     /* 3*n^3 */
    seq_t term2 = (seq_t)21 * n2;    /* 21*n^2 */

    /* Compute 3*n^3 + 21*n^2 + 14 first, then subtract 6*n. */
    seq_t sum = term1 + term2;
    sum += (seq_t)14;
    sum -= (seq_t)6 * nn;           /* subtract 6*n */

    /* Final division by 2 (even number, safe). */
    return sum / (seq_t)2;
}
