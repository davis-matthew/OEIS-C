/*
 * A001384  Number of n-node trees of height at most 4.
 * https://oeis.org/A001384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   rooted trees counted by height. R_0(x) = x, and for h >= 1
 *            R_h(x) = x * EulerT(R_{h-1})(x) = x * Prod_{k>=1}
 *            1/(1-x^k)^{r_{h-1}(k)}. a(n) = [x^n] R_4(x) for n >= 1, and a(0)
 *            = 1. Equivalently, a(n+1) is the Euler transform of A001383
 *            (shift right). Euler transform b of c: b(0) = 1, b(n) = (1/n)
 *            S...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   a1384_euler_div, a1384_euler_mc, a1384_gcd, a1384_is_fixed,
 *            a1384_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a1384_euler_div.h"
#include "../utils/a1384_euler_mc.h"
#include "../utils/a1384_gcd.h"
#include "../utils/a1384_is_fixed.h"
#include "../utils/a1384_zeros.h"

static seq_t *a1384_cache = 0;
static idx_t a1384_cache_n = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a1384_cache != 0 && n <= a1384_cache_n) return a1384_cache[n];

    int fixed = a1384_is_fixed();
    if (fixed && n > 1000) return 0; /* value cannot fit in any fixed width */

    idx_t N = n;
    seq_t *r = a1384_zeros(N + 1);
    seq_t *e = a1384_zeros(N + 1);
    idx_t m;
    int lev;
    r[1] = 1; /* R_0 = x */
    for (lev = 1; lev <= 4; lev++) {
        if (fixed) a1384_euler_mc(r, e, N - 1);
        else a1384_euler_div(r, e, N - 1);
        r[0] = 0;
        for (m = 1; m <= N; m++) r[m] = e[m - 1];
    }
    free(e);
    r[0] = 1;
    if (a1384_cache != 0) free(a1384_cache);
    a1384_cache = r;
    a1384_cache_n = N;
    return r[n];
}
