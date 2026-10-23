/*
 * A166828  Number of n X 2 1..6 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((n-1)*(n-2)*(n^8 + 53*n^7 + 1342*n^6 + 17630*n^5 * +
 *            91114*n^4 - 102013*n^3 - 1246977*n^2 + 1575810*n - 113400)) * /
 *            226800 * This implements the explicit polynomial formula that
 *            was empirically * observed for the sequence A166828
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) and a(2) are zero by definition */
    if (n < 3)
        return 0;

    /* work on the index as seq_t to preserve arbitrary-precision */
    seq_t ns = (seq_t)n;

    /* Evaluate the 8th‑degree polynomial by Horner’s rule */
    seq_t term = (seq_t)1;
    term = term * ns + (seq_t)53;
    term = term * ns + (seq_t)1342;
    term = term * ns + (seq_t)17630;
    term = term * ns + (seq_t)91114;
    term = term * ns - (seq_t)102013;      /* negative coefficient */
    term = term * ns - (seq_t)1246977;     /* negative coefficient */
    term = term * ns + (seq_t)1575810;
    term = term * ns - (seq_t)113400;      /* final constant */

    /* Multiply by the front factors (n-1)(n-2) */
    seq_t numerator = (ns - (seq_t)1) * (ns - (seq_t)2);
    numerator *= term;

    /* Final division by 226800 */
    const seq_t denom = (seq_t)226800;
    return numerator / denom;
}
