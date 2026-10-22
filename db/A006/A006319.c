/*
 * A006319  Royal paths in a lattice (convolution of A006318).
 * https://oeis.org/A006319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A006318(n) - A006318(n-1) for n >= 1, a(0) = 1, since
 *            G.f. A(x) = (1-x)*S(x) where S(x) is the g.f. of the large
 *            Schroeder numbers S(n) = A006318(n): (n+1)*S(n) =
 *            3*(2n-1)*S(n-1) - (n-2)*S(n-2), S(0) = 1, S(1) = 2. For
 *            fixed-width seq_t this uses the overflow-safe recurrence
 *            instead: a(n)...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *S_cache = NULL;
static idx_t S_cap = 0;
static idx_t S_cnt = 0;

static void ensure_S(idx_t n) {
    if (n < S_cnt) return;
    if (n + 1 > S_cap) {
        idx_t newcap = n + 1;
        if (newcap < 2 * S_cap) newcap = 2 * S_cap;
        if (newcap < 2) newcap = 2;
        if (S_cache == NULL) {
            S_cache = alloc_zero(newcap);
        } else {
            S_cache = grow_zero(S_cache, S_cap, newcap);
        }
        S_cap = newcap;
    }
    if (S_cnt == 0) {
        S_cache[0] = 1;
        S_cache[1] = 2;
        S_cnt = 2;
    }
    for (idx_t i = S_cnt; i <= n; i++) {
        seq_t t = (seq_t)(3 * (2 * i - 1)) * S_cache[i - 1];
        t -= (seq_t)(i - 2) * S_cache[i - 2];
        t /= (seq_t)(i + 1);
        S_cache[i] = t;
    }
    S_cnt = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;

    if (seq_is_fixed_width()) {
        seq_t *c = alloc_zero(n + 1);
        c[0] = 1;
        c[1] = 1;
        for (idx_t i = 2; i <= n; i++) {
            seq_t r = c[i - 1] * 4;
            for (idx_t k = 2; k + 2 <= i; k++) {
                r += c[k] * c[i - k - 1];
            }
            c[i] = r;
        }
        seq_t res = c[n];
        release(c, n + 1);
        return res;
    }

    ensure_S(n);
    return S_cache[n] - S_cache[n - 1];
}
