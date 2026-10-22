/*
 * A003728  Expansion of e.g.f. log(1+x*cos(x)).
 * https://oeis.org/A003728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   with g(x) = x*cos(x), G(j) = j*(-1)^((j-1)/2) for odd j and 0
 *            for even j, (1+g)*f' = g' gives a(0) = 0 and a(m) = G(m) -
 *            Sum_{j=1..m-1, j odd} binomial(m-1,j)*G(j)*a(m-j)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   Gc, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/Gc.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < memo_len) return memo[n];
    idx_t newlen = n + 1;
    memo = grow_zero(memo, memo_len, newlen);
    idx_t start = memo_len;
    if (start == 0) {
        memo[0] = (seq_t)0;
        start = 1;
    }
    for (idx_t m = start; m <= n; m++) {
        idx_t N = m - 1;
        seq_t s = Gc(m);
        seq_t b = (seq_t)1; /* binomial(N, j) */
        for (idx_t j = 1; j <= N; j++) {
            b = b * (seq_t)(N - j + 1) / (seq_t)j;
            if (j & 1) s -= b * Gc(j) * memo[m - j];
        }
        memo[m] = s;
    }
    memo_len = newlen;
    return memo[n];
}
