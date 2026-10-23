/*
 * A166815  Number of nX2 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6+18*n^5+115*n^4+60*n^3-566*n^2+372*n)/180
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula gives 0, which matches the offset of the
       sequence (a(1) = 0). */
    if (n == 0) return (seq_t)0;

    /* Use 128‑bit arithmetic for the intermediate calculation to avoid
       overflow for all n that fit in the provided seq_t type.  The final
       result is then cast back to seq_t. */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 num;

    num = N * N * N * N * N * N;                        /* n^6 */
    num += (unsigned __int128)18 * N * N * N * N * N;   /* +18 n^5 */
    num += (unsigned __int128)115 * N * N * N * N;      /* +115 n^4 */
    num += (unsigned __int128)60 * N * N * N;           /* +60 n^3 */
    num -= (unsigned __int128)566 * N * N;              /* -566 n^2 */
    num += (unsigned __int128)372 * N;                 /* +372 n */

    num /= 180; /* exact division for all integer n */

    return (seq_t)num;
}
