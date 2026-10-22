/*
 * A006710  Expansion of eta(q^10)^12/(eta(q^2)^4*eta(q^5)^8) in powers of q.
 * https://oeis.org/A006710
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^(n-3)] Product_{k>=1} 1/(1-q^k)^b(k), where b is the
 *            period-10 sequence [0,4,0,4,8,4,0,4,0,0] (Euler transform), i.e.
 *            the expansion of eta(q^10)^12/(eta(q^2)^4*eta(q^5)^8)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 65, u64 n <= 193,
 *            u128 n <= 639, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = 0;
static idx_t clen = 0;

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t m = n - 3;
    if (m >= clen) {
        idx_t N = m + 1;
        if (N < 2 * clen) N = 2 * clen;
        if (N < 64) N = 64;
        if (cache) release(cache, clen);
        cache = alloc_zero(N);
        clen = N;
        cache[0] = 1;
        for (idx_t k = 1; k < N; k++) {
            idx_t r = k % 10;
            int e = (r == 5) ? 8 : ((r % 2 == 0 && r != 0) ? 4 : 0);
            for (int t = 0; t < e; t++)
                for (idx_t i = k; i < N; i++)
                    cache[i] += cache[i - k];
        }
    }
    return cache[m];
}
