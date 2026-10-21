/*
 * A221217  T(n,k) = ((n+k)^2-2*n+3-(n+k-1)*(1+2*(-1)^(n+k)))/2; n , k > 0,
 *          read by antidiagonals.
 * https://oeis.org/A221217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 230, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221217: a(n) = T(i,j) where T(n,k)=((n+k)^2-2*n+3-(n+k-1)*(1+2*(-1)^(n+k)))/2
   and (i,j) are the coordinates read by antidiagonals of the positive grid.
   That is, the n‑th term corresponds to the pair (i,j) with i+j equal to
   the least s such that n ≤ (s-1)*s/2, and i = (n - (s-2)*(s-1)/2).
*/

#include <stdint.h>

/* integer square root of a 64-bit value: floor(sqrt(v)) */
static idx_t isqrt_uint64(idx_t v)
{
    idx_t low = 0, high = v;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v) { ans = mid; low = mid + 1; }
        else high = mid - 1;
    }
    return ans;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard for safety */

    /* Find diagonal sum s such that n is within diagonal s */
    idx_t t = isqrt_uint64(1 + 8 * n);              /* floor sqrt(1+8n) */
    idx_t s = (1 + t) / 2;                          /* initial guess */

    /* Adjust s to satisfy n <= (s-1)*s/2 */
    while ((__uint128_t)s * (s - 1) / 2 < n) s++;
    while ((__uint128_t)(s - 1) * (s - 2) / 2 >= n) s--;

    /* Number of terms before diagonal s */
    idx_t cumprev = (s - 1) * (s - 2) / 2;

    /* Position inside diagonal s, zero‑based */
    idx_t pos = n - cumprev - 1;

    idx_t i = pos + 1;          /* row index  (1‑based)   */
    idx_t j = s - i;            /* column index (1‑based) */

    /* Compute T(i,j) using unsigned arithmetic */
    seq_t ii = i;
    seq_t jj = j;
    seq_t ij = ii + jj;                /* n+k */
    seq_t numerator;

    if ((ij & 1) == 0) {               /* ij even: 1+2*(-1)^ij = 3 */
        numerator = ij * ij
                   - 2 * ii
                   - 3 * ij
                   + (seq_t)6;
    } else {                           /* ij odd: 1+2*(-1)^ij = -1 */
        numerator = ij * ij
                   - 2 * ii
                   + ij
                   + (seq_t)2;
    }

    return numerator / 2;
}
