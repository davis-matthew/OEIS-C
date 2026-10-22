/*
 * A004307  Number of permutations p of [n] such that (n-p(i)+i) mod n >= 4
 *          for all i.
 * https://oeis.org/A004307
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j * r_j(n) * (n-j)!, where r_j(n) is
 *            the number of ways to place j non-attacking rooks on the cyclic
 *            band {(i, i+d mod n) : d = 0..3}. The r_j(n) come from a
 *            transfer-matrix DP over the rows, which tracks column occupancy
 *            and the columns that wrap around
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    idx_t L = n + 2;
    idx_t total = (idx_t)64 * L;
    seq_t *cur = alloc_zero(total);
    seq_t *nxt = alloc_zero(total);
    idx_t s, m, i, j, d, k;

    /* state index = S*8 + m; S = real columns 0..2 reserved for wrap-around rooks,
       m = occupancy of columns i, i+1, i+2 (virtual) before row i */
    for (s = 0; s < 8; ++s) cur[(s * 8 + s) * L] = 1;

    for (i = 0; i < n; ++i) {
        for (k = 0; k < 64; ++k)
            for (j = 0; j <= i + 1 && j < L; ++j)
                nxt[k * L + j] = 0;
        for (s = 0; s < 8; ++s) {
            for (m = 0; m < 8; ++m) {
                seq_t *src = cur + (s * 8 + m) * L;
                for (j = 0; j <= i; ++j) {
                    if (src[j] == 0) continue;
                    unsigned w = (unsigned)m; /* bit 3 (column i+3) is always free */
                    nxt[(s * 8 + (w >> 1)) * L + j] += src[j];
                    for (d = 0; d < 4; ++d) {
                        if (!((w >> d) & 1u)) {
                            unsigned w2 = w | (1u << d);
                            nxt[(s * 8 + (w2 >> 1)) * L + j + 1] += src[j];
                        }
                    }
                }
            }
        }
        seq_t *t = cur; cur = nxt; nxt = t;
    }

    /* r_j = sum over S of DP[S][m == S] */
    seq_t pos = 0, neg = 0;
    seq_t f = 1; /* (n-j)! */
    for (j = n + 1; j-- > 0;) {
        if (j < n) f *= (seq_t)(n - j);
        seq_t r = 0;
        for (s = 0; s < 8; ++s) r += cur[(s * 8 + s) * L + j];
        if (r == 0) continue;
        if (j % 2 == 0) pos += r * f;
        else neg += r * f;
    }

    release(cur, total);
    release(nxt, total);
    return pos - neg;
}
