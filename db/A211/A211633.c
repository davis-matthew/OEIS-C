/*
 * A211633  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2>2x^2+2y^2.
 * https://oeis.org/A211633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * ∑_{w=1..n} |{ (x,y) : 1≤x,y≤n , 2x²+2y² < w² }|
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t total = 0;

    for (idx_t w = 1; w <= n; ++w) {
        idx_t w2 = w * w;
        /* Since 2x²+2y² ≥ 4 for x,y≥1, skip w with w²≤4 */
        if (w2 <= 4)
            continue;

        /* Compute M = floor((w²-1)/2)  :  x²+y² ≤ M  */
        idx_t M = (w2 - 1) / 2;

        /* Upper bound for x */
        idx_t max_x = (idx_t)sqrt((double)M);
        if (max_x > n)
            max_x = n;

        for (idx_t x = 1; x <= max_x; ++x) {
            idx_t xsq = x * x;
            if (xsq > M)
                break;

            idx_t cur = M - xsq;            /* y² ≤ cur  */
            idx_t y_max = (idx_t)sqrt((double)cur);
            if (y_max > n)
                y_max = n;

            total += (seq_t)y_max;
        }
    }

    return total * (seq_t)8;  /* 2³ sign choices for each non‑zero coordinate */
}
