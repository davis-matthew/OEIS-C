/*
 * A001981  Restricted partitions.
 * https://oeis.org/A001981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(4n)] of the Gaussian binomial [n+8 choose 8]_q =
 *            prod_{j=1..8} (1-q^(n+j))/(1-q^j), built step by step: [n+j
 *            choose j]_q = [n+j-1 choose j-1]_q * (1-q^(n+j)) / (1-q^j)
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 972 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t N = 4 * n;
    idx_t j, k;
    seq_t r;
    seq_t *c = mem_new(seq_t, (size_t)(N + 1));
    if (!c) return 0;
    c[0] = 1;
    for (k = 1; k <= N; k++) c[k] = 0;
    for (j = 1; j <= 8; j++) {
        idx_t s = n + j;
        /* divide by (1 - q^j): all coefficients stay nonnegative */
        for (k = j; k <= N; k++) c[k] += c[k - j];
        /* multiply by (1 - q^(n+j)): the result is [n+j choose j]_q, nonnegative */
        if (s <= N) {
            for (k = N; k >= s; k--) c[k] -= c[k - s];
        }
    }
    r = c[N];
    mem_free(c);
    return r;
}
