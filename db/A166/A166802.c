/*
 * A166802  Number of nX3 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1)*(181440 - 405504*n + 100296*n^2 + 573392*n^3 +
 *            307532*n^4 * + 67775*n^5 + 9815*n^6 + 569*n^7 + 29*n^8) / 181440
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = n;          /* convert n to seq_t */
    seq_t Nm1 = N - 1;     /* (n-1)    */
    if (Nm1 == 0)          /* n = 1 → 0 */
        return 0;

    /* pre‑compute powers of n */
    seq_t n2 = N * N;          /* n^2  */
    seq_t n3 = n2 * N;         /* n^3  */
    seq_t n4 = n3 * N;         /* n^4  */
    seq_t n5 = n4 * N;         /* n^5  */
    seq_t n6 = n5 * N;         /* n^6  */
    seq_t n7 = n6 * N;         /* n^7  */
    seq_t n8 = n7 * N;         /* n^8  */

    /* evaluate the polynomial part */
    seq_t term = 181440;          /* constant term */
    term -= 405504 * N;
    term += 100296 * n2;
    term += 573392 * n3;
    term += 307532 * n4;
    term += 67775  * n5;
    term += 9815   * n6;
    term += 569    * n7;
    term += 29     * n8;

    /* final computation */
    seq_t numer = Nm1 * term;
    return numer / 181440;        /* division is exact */
}
