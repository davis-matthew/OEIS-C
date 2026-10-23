/*
 * A154684  Triangle read by rows where T(m,n)=2mn + m + n - 3, 1<=n<=m.
 * https://oeis.org/A154684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A154684
   Triangle read by rows where T(m,n)=2mn + m + n - 3, 1<=n<=m.
   Flattened sequence a(n) is obtained by reading the triangle row by row:
   a(n) = 2*m*k + m + k - 3
   where m is the unique integer with 
          m(m+1)/2 >= n  and  (m-1)m/2 < n,
   and k = n - (m-1)m/2  (the column index within row m).
*/

#include <stdint.h>

/* integer square root for uint64_t, returns floor(sqrt(x)) */
static idx_t isqrt64(idx_t x)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of 4 <= 2^64 */
    while (bit > x) bit >>= 2;

    while (bit) {
        idx_t temp = res + bit;
        if (temp <= x) {
            res = temp;
            x  -= temp;
        }
        res >>= 1;
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; a(0) is undefined but return 0 for safety */
        return 0;

    /* Find row m: smallest m with m*(m+1)/2 >= n */
    idx_t val = (n << 3) + 1;               /* 8*n + 1, computed modulo 2^64 */
    idx_t s   = isqrt64(val);               /* floor(sqrt(8*n+1))          */
    idx_t m   = (s - 1) / 2;                /* initial estimate           */

    /* Correct possible off-by-one due to truncation */
    while (m * (m + 1) / 2 < n) ++m;
    while (m > 0 && (m - 1) * m / 2 >= n) --m;

    /* Column index within the row */
    idx_t prior = (m - 1) * m / 2;          /* elements before row m        */
    idx_t k = n - prior;                    /* 1‑based column index         */

    /* Compute T(m,k) = 2*m*k + m + k - 3   */
    seq_t M = (seq_t)m;
    seq_t K = (seq_t)k;
    return 2 * M * K + M + K - 3;
}
