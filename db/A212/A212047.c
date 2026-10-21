/*
 * A212047  Number of (w,x,y,z) with all terms in {1,...,n} and w*x=4*y*z.
 * https://oeis.org/A212047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count all ordered quadruples (w,x,y,z) with 1≤w,x,y,z≤n and
   w*x = 4*y*z.  The implementation pre‑computes, for each
   q, the number of ordered pairs (y,z) with y*z = q and then
   sums these counts for all pairs (w,x) whose product is a
   multiple of 4. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 0 */
        return 0;

    /* Maximum possible value of y*z when 1≤y,z≤n is n*n/4
       (half the largest product w*x/4). */
    size_t maxq = (size_t)n * (size_t)n / 4;

    /* f[q] stores the number of ordered pairs (y,z) such that
       y*z = q, with 1 ≤ y,z ≤ n. */
    uint16_t *f = (uint16_t *)calloc(maxq + 1, sizeof(uint16_t));
    if (!f) /* allocation failed – unlikely, but handle gracefully */
        return 0;

    /* Build f[q].  The two nested loops are bounded by n, but the
       inner loop breaks as soon as y*z exceeds maxq. */
    for (idx_t y = 1; y <= n; ++y) {
        for (idx_t z = 1; z <= n; ++z) {
            idx_t prod = y * z;
            if (prod > maxq)
                break;
            ++f[prod];
        }
    }

    /* Now count all pairs (w,x) and add the corresponding
       f[w*x/4] when w*x is divisible by 4. */
    seq_t ans = 0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t prod = w * x;
            if ((prod & 3) == 0) {          /* prod % 4 == 0 */
                idx_t q = prod >> 2;         /* q = prod / 4 */
                ans += (seq_t)f[q];
            }
        }
    }

    free(f);
    return ans;
}
