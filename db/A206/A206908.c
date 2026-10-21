/*
 * A206908  a(n) = 4*n + floor(n/sqrt(3)).
 * https://oeis.org/A206908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + floor(n/sqrt(3))
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n / sqrt(3)) by binary search on the inequality
       3*m*m <= n*n, which is equivalent to m <= n / sqrt(3).  */
    idx_t lo = 0, hi = n;          /* lo, hi are in the same type as n     */
    idx_t m  = 0;                  /* best candidate found so far          */
    __int128 n2 = (__int128)n * n; /* n squared, fits in 128 bits for all
                                      n that comfortably fit in idx_t      */

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 val = (__int128)mid * mid * 3;
        if (val <= n2) {           /* mid is still <= floor(n/sqrt(3))   */
            m = mid;
            lo = mid + 1;
        } else {                   /* mid is too large                     */
            if (mid == 0) break;   /* safety for zero value                */
            hi = mid - 1;
        }
    }

    /* Final formula: 4*n + floor(n/sqrt(3)) */
    seq_t result = (seq_t)(4 * (seq_t)n + (seq_t)m);
    return result;
}
