/*
 * A211646  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2<3n.
 * https://oeis.org/A211646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with all terms in
 *            {1,…,n} and w^2+x^2+y^2<3n
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit integer.
   Returns floor(sqrt(n)).  Uses Newton's method.  */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    if (x == 0) return 0;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Threshold T = 3n.  Use idx_t to avoid overflow in intermediate
       comparisons.  */
    idx_t T = n * (idx_t)3;
    if (T <= 1) return 0;

    seq_t count = 0;

    /* Maximum w such that w^2 < T.  */
    idx_t max_w = isqrt_u64(T - 1);
    if (max_w > n) max_w = n;

    for (idx_t w = 1; w <= max_w; ++w) {
        idx_t w2 = w * w;
        /* Remaining value after subtracting w^2.  */
        idx_t rem_w = T - w2;
        if (rem_w <= 1) continue;              /* No x will satisfy w^2+x^2< T */

        /* Maximum x such that w^2+x^2 < T.  */
        idx_t max_x = isqrt_u64(rem_w - 1);
        if (max_x > n) max_x = n;

        for (idx_t x = 1; x <= max_x; ++x) {
            idx_t x2 = x * x;
            idx_t rem = T - w2 - x2;
            if (rem <= 1) continue;           /* No y will satisfy */

            /* Maximum y such that y^2 < rem.  */
            idx_t y_max = isqrt_u64(rem - 1);
            if (y_max > n) y_max = n;

            count += (seq_t)y_max;
        }
    }

    return count;
}
