/*
 * A166798  Number of nX2 1..5 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 32*n^7 + 476*n^6 + 3038*n^5 - 1141*n^4 - 35182*n^3
 *            * + 53584*n^2 - 25848*n + 5040)/5040
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to prevent overflow when seq_t is 64‑bit. */
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 res = 1;          /* coefficient of n^8 */
    res = res * tn + 32;                /* +32*n^7 */
    res = res * tn + 476;               /* +476*n^6 */
    res = res * tn + 3038;              /* +3038*n^5 */
    res = res * tn - 1141;              /* -1141*n^4 */
    res = res * tn - 35182;             /* -35182*n^3 */
    res = res * tn + 53584;             /* +53584*n^2 */
    res = res * tn - 25848;             /* -25848*n  */
    res = res * tn + 5040;              /* +5040       */
    res /= 5040;                        /* final division */
    return (seq_t)res;
}
