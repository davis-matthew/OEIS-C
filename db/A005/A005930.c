/*
 * A005930  Theta series of D_5 lattice.
 * https://oeis.org/A005930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = r_5(2n), the coefficient of q^(2n) in theta_3(q)^5 =
 *            (Sum_{k=-oo..oo} q^(k^2))^5; series computed by repeated
 *            multiplication by theta_3
 * Range:     u8 n <= 4, u16 n <= 122, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t cache_len = 0; /* number of coefficients stored (indices 0..cache_len-1) */

static void build(idx_t len) {
    seq_t *p = alloc_zero(len);
    idx_t i, k, rep;
    /* p = theta_3(q) */
    p[0] = 1;
    for (k = 1; k * k < len; k++) p[k * k] = 2;
    /* multiply by theta_3 four more times, in place (descending index) */
    for (rep = 0; rep < 4; rep++) {
        i = len;
        while (i > 0) {
            i--;
            seq_t s = p[i];
            for (k = 1; k * k <= i; k++) {
                s += p[i - k * k] * 2;
            }
            p[i] = s;
        }
    }
    if (cache) release(cache, cache_len);
    cache = p;
    cache_len = len;
}

seq_t a(idx_t n) {
    idx_t need = 2 * n + 1;
    if (need > cache_len) {
        idx_t len = cache_len * 2;
        if (len < need) len = need;
        if (len < 64) len = 64;
        build(len);
    }
    return cache[2 * n];
}
