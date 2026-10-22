/*
 * A003268  Central Fibonomial coefficients.
 * https://oeis.org/A003268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonomial(n+2, k) = Product_{i=1..k} F(n+2-k+i)/F(i), k
 *            = floor((n+2)/2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   a3268_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3268_g.h"

seq_t a(idx_t n) {
    idx_t m = n + 2;
    idx_t k = m / 2;
    idx_t start = m - k + 1;

    /* fb = F(i), fb1 = F(i+1), starting at i = 1 */
    seq_t fb = 1, fb1 = 1;
    /* ft = F(top), ft1 = F(top+1), starting at top = start */
    seq_t ft = 1, ft1 = 1;
    for (idx_t j = 1; j < start; j++) {
        seq_t t = ft + ft1;
        ft = ft1;
        ft1 = t;
    }

    seq_t r = 1;
    for (idx_t i = 1; i <= k; i++) {
        /* r * ft / fb is an exact integer; cancel first to avoid overflow */
        seq_t g = a3268_g(r, fb);
        seq_t q = fb / g;
        r = (r / g) * (ft / q);

        seq_t t = fb + fb1;
        fb = fb1;
        fb1 = t;
        if (i < k) {
            t = ft + ft1;
            ft = ft1;
            ft1 = t;
        }
    }
    return r;
}
