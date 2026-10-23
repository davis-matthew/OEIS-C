/*
 * A166827  Number of nX2 1..5 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (n^8+32*n^7+420*n^6+1862*n^5-4431*n^4-23002*n^3+59450*n^2-39372*n+5040)
 *            / 5040
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not defined by the offset; return 0 for safety */
    if (n == 0) return 0;

    /* Use 128‑bit intermediate to avoid overflow when n is large
       (the numerator fits in 128 bits for n ≤ 10 000). */
    __int128 N  = (unsigned long long)n;
    __int128 tmp = 1;             /* coefficient of n^8 */
    tmp = tmp * N + 32;           /* +32*n^7   */
    tmp = tmp * N + 420;          /* +420*n^6  */
    tmp = tmp * N + 1862;         /* +1862*n^5 */
    tmp = tmp * N - 4431;         /* -4431*n^4 */
    tmp = tmp * N - 23002;        /* -23002*n^3 */
    tmp = tmp * N + 59450;        /* +59450*n^2 */
    tmp = tmp * N - 39372;        /* -39372*n  */
    tmp = tmp * N + 5040;         /* +5040     */

    tmp /= 5040;                  /* exact division */

    return (seq_t)tmp;
}
