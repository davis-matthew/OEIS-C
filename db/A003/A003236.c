/*
 * A003236  a(n) = Sum_{k=0..n} (-1)^(n-k) C(n,k)*C((k+1)^2, n).
 * https://oeis.org/A003236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) C(n,k)*C((k+1)^2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   a3236_binom_big, a3236_binom_mod, a3236_inv, a3236_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3236_binom_big.h"
#include "../utils/a3236_binom_mod.h"
#include "../utils/a3236_inv.h"
#include "../utils/a3236_width.h"

seq_t a(idx_t n) {
    int w = a3236_width();
    seq_t pos = 0, neg = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        idx_t m = (k + 1) * (k + 1);
        if (m < n) continue;
        seq_t term;
        if (w) term = a3236_binom_mod(n, k, w) * a3236_binom_mod(m, n, w);
        else term = a3236_binom_big(n, k) * a3236_binom_big(m, n);
        if (((n - k) & 1) == 0) pos += term;
        else neg += term;
    }
    return pos - neg;
}
