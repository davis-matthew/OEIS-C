/*
 * A204267  Symmetric matrix: f(i,j)=(i+j+1 mod 3), by antidiagonals.
 * https://oeis.org/A204267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k mod 3), where k is the diagonal index containing the
 *            n‑th entry in the antidiagonal enumeration of the infinite
 *            matrix f(i,j) = (i+j) mod 3. The diagonal index k is the unique
 *            integer with (k+1)(k+2)/2 ≥ n > k(k+1)/2. Thus a(n) depends only
 *            on k, not on the row/column coordinates
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest m such that m(m+1)/2 ≥ n.  Then k = m-1. */
    idx_t low  = 1;                      /* with n≥1, m≥1 is always true   */
    idx_t high = 4294967295ULL;          /* 2^32-1, large enough for n<2^64 */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t m = low;            /* minimal m: m(m+1)/2 ≥ n                         */
    idx_t k = m - 1;          /* diagonal index                                       */
    seq_t result = (seq_t)(k % 3);
    return result;
}
