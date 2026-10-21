/*
 * A095876  Triangle read by rows: T(n,k) = k^(n^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k^(r^2), where r is the unique integer such that *
 *            r*(r−1)/2 < n ≤ r*(r+1)/2 and k = n − r*(r−1)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Helpers:   int_sqrt, pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_int_seq.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; for safety handle out‑of‑range */
    if (n < 1)
        return 0;

    /* determine row r using the inverse of the triangular numbers */
    idx_t temp = 8 * n + 1;            /* 8n+1 fits in idx_t for n up to ~2^61 */
    idx_t s    = int_sqrt(temp);       /* floor(sqrt(8n+1)) */
    idx_t r    = (s - 1) / 2;           /* initial estimate */

    /* minor adjustment to guarantee r*(r+1)/2 ≥ n and (r-1)*r/2 < n */
    while (r * (r + 1) / 2 < n) r++;
    while (r > 0 && r * (r - 1) / 2 >= n) r--;

    /* column k within that row */
    idx_t k = n - r * (r - 1) / 2;

    /* k is at most r, so it fits safely in seq_t */
    seq_t base = (seq_t)k;
    if (k == 1)               /* 1^anything = 1, avoid a slow power call */
        return (seq_t)1;

    idx_t exp = r * r;         /* exponent r^2 fits in idx_t for n ≤ 10000 */
    return pow_int_seq(base, exp);
}
