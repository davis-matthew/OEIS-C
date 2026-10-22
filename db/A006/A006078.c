/*
 * A006078  Number of triangulated (n+2)-gons rooted at an exterior edge.
 * https://oeis.org/A006078
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (C(n+1) - 2*C(n) + [n even]*3*C(n/2)) / 4, where C(k) =
 *            Catalan numbers; computed as 4*a(n) = 2*Sum_{k=1..ceil(n/2)-1}
 *            C(k)*C(n-k) + [n even]*(C(n/2)^2 + 3*C(n/2))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   cat_next, cat_prev, sgcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cat_next.h"
#include "../utils/cat_prev.h"
#include "../utils/sgcd_u64.h"

seq_t a(idx_t n) {
    seq_t c = 1, cm = 0, Q = 0, q = 0, lo, hi, res;
    long long r = 0, rest;
    idx_t j, k, m = n / 2;

    for (j = 1; j <= n - 1; j++) {
        c = cat_next(c, j);
        if (n % 2 == 0 && j == m) cm = c;
    }
    hi = c;          /* C(n-1) */
    lo = 1;          /* C(1) */
    for (k = 1; 2 * k < n; k++) {
        seq_t t = lo * hi;
        q += t / 4;
        r += (long long)(t % 4);
        lo = cat_next(lo, k + 1);
        hi = cat_prev(hi, n - k);
    }
    if (n % 2 == 0) Q = cm * (cm + 3);
    /* 4a = 2*(4q + r) + Q */
    rest = 2 * r + (long long)(Q % 4);
    res = 2 * q + Q / 4 + (seq_t)(rest / 4);
    return res;
}
