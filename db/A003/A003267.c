/*
 * A003267  Central Fibonomial coefficients.
 * https://oeis.org/A003267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} F(n+k)/F(k), with F = Fibonacci
 *            (A000045); each partial product C(n+j,j)_F is an integer (a
 *            Fibonomial), so the running product is divided exactly at each
 *            step (after cancelling a gcd)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   a3267_gcd, a3267_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3267_gcd.h"
#include "../utils/a3267_width.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    int w = a3267_width();
    if (w < 200) {
        /* largest n with a(n) representable in w bits */
        idx_t lim = (w <= 8) ? 3 : (w <= 16) ? 4 : (w <= 32) ? 6 : (w <= 64) ? 9 : 13;
        if (n > lim) return 0;
    }

    /* fa = F(j), fb = F(j+1), starting at j = 0 */
    seq_t fa = 0, fb = 1;
    /* ga = F(n+j), gb = F(n+j+1), starting at j = 0 */
    seq_t ga = 0, gb = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t t = ga + gb;
        ga = gb;
        gb = t;
    }

    seq_t r = 1;
    for (idx_t j = 1; j <= n; j++) {
        seq_t t = fa + fb;
        fa = fb;
        fb = t;
        t = ga + gb;
        ga = gb;
        gb = t;
        /* r = r * F(n+j) / F(j), done without exceeding the result size */
        seq_t g = a3267_gcd(fa, r % fa);
        r /= g;
        seq_t d = fa / g;
        r = r * (ga / d);
    }
    return r;
}
