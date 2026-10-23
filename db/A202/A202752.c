/*
 * A202752  Number of n X 4 nonnegative integer arrays with each row and
 *          column increasing from zero by 0 or 1.
 * https://oeis.org/A202752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/360)*n^6 + (1/30)*n^5 + (5/72)*n^4 - (1/6)*n^3 +
 *            (77/180)*n^2 + (19/30)*n Combined into an integer division: a(n)
 *            = (n^6 + 12*n^5 + 25*n^4 - 60*n^3 + 154*n^2 + 228*n) / 360
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once */
    const seq_t N = (seq_t)n;

    /* Compute successive powers of n in seq_t arithmetic */
    seq_t n_sq  = N * N;          /* n^2  */
    seq_t n_3   = n_sq * N;       /* n^3  */
    seq_t n_4   = n_3 * N;        /* n^4  */
    seq_t n_5   = n_4 * N;        /* n^5  */
    seq_t n_6   = n_5 * N;        /* n^6  */

    /* Build the numerator using integer multiplications */
    seq_t num = n_6                /* + 1*n^6    */
              + (seq_t)12 * n_5   /* +12*n^5    */
              + (seq_t)25 * n_4   /* +25*n^4    */
              - (seq_t)60 * n_3   /* -60*n^3    */
              + (seq_t)154 * n_sq/* +154*n^2   */
              + (seq_t)228 * N;   /* +228*n     */

    /* Final division by 360 gives the exact term */
    return num / (seq_t)360;
}
