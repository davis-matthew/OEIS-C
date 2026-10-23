/*
 * A164754  Number of n X 2 1..4 arrays with all 1's connected, all 2's
 *          connected, all 3's connected, all 4's connected, 1 in the upper
 *          left corner, 2 in the upper right corner, 3 in the lower left
 *          corner, 4 in the lower right corner, and with no element having
 *          more than 2 neighbors with the same value.
 * https://oeis.org/A164754
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 - 11*n + 5 for n >= 3; a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        /* for n == 2 the sequence defines a(2) = 1 */
        /* values below the offset are not required; return 0 */
        return (n == 2) ? (seq_t)1 : (seq_t)0;
    }
    /* use idx_t (uint64_t) for intermediate calculations to avoid
       overflow on the intermediate 64‑bit arithmetic;
       final cast to seq_t ensures correct type for the return value. */
    idx_t N   = n;
    idx_t val = 4 * N * N - 11 * N + 5;
    return (seq_t)val;
}
