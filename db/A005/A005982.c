/*
 * A005982  3 up, 3 down, 3 up, ... permutations of length 3n+1.
 * https://oeis.org/A005982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of length 3n+1 with up-down
 *            signature UUUDDDUUUDDD... (gap t is an ascent iff t mod 6 < 3),
 *            computed by the Entringer-type DP: f_1[1] = 1; at an ascent g[j]
 *            = Sum_{k<j} f[k], at a descent g[j] = Sum_{k>=j} f[k], j =
 *            1..i+1; a(n) = Sum_j f_{3n+1}[j]
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/grow_zero.h"

static seq_t *memo = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];

    idx_t L = 3 * n + 1;
    seq_t *f = alloc_zero(L + 2);
    seq_t *g = alloc_zero(L + 2);
    memo = grow_zero(memo, memo_len, n + 1);

    f[1] = 1;
    idx_t i = 1;
    for (idx_t t = 0; t + 1 < L; t++) {
        seq_t s = 0;
        if (t % 6 < 3) {
            for (idx_t j = 1; j <= i + 1; j++) {
                g[j] = s;
                if (j <= i) s += f[j];
            }
        } else {
            for (idx_t j = i + 1; j > 0; j--) {
                if (j <= i) s += f[j];
                g[j] = s;
            }
        }
        seq_t *tmp = f; f = g; g = tmp;
        i++;
        if ((i - 1) % 3 == 0) {
            idx_t m = (i - 1) / 3;
            seq_t tot = 0;
            for (idx_t j = 1; j <= i; j++) tot += f[j];
            memo[m] = tot;
        }
    }

    release(f, L + 2);
    release(g, L + 2);
    memo_len = n + 1;
    return memo[n];
}
