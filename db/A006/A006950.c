/*
 * A006950  G.f.: Product_{k>=1} (1 + x^(2*k - 1)) / (1 - x^(2*k)).
 * https://oeis.org/A006950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts not congruent to 2 mod 4;
 *            G.f.: Product_{k>=1, k mod 4 != 2} 1/(1 - x^k), a(0) = 1
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 171, u64 n <= 557,
 *            u128 n <= 1947, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = 0;
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t newcap = n + 1;
        if (newcap < 2 * cap) newcap = 2 * cap;
        if (newcap < 64) newcap = 64;
        seq_t *f = alloc_zero(newcap);
        f[0] = 1;
        for (idx_t k = 1; k < newcap; k++) {
            if (k % 4 == 2) continue;
            for (idx_t i = k; i < newcap; i++) {
                f[i] += f[i - k];
            }
        }
        if (memo) release(memo, cap);
        memo = f;
        cap = newcap;
    }
    return memo[n];
}
