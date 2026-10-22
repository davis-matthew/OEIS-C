/*
 * A002295  Number of dissections of a polygon: binomial(6n,n)/(5n+1).
 * https://oeis.org/A002295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = binomial(6n,n)/(5n+1) = (6n)!/(n!*(5n+1)!), computed by
 *            multiplying out its prime factorization (Legendre's formula)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   leg_a002295
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/leg_a002295.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    uint64_t N = 6 * (uint64_t)n;
    uint64_t M = 5 * (uint64_t)n + 1;
    char *comp = (char *)malloc((size_t)(N + 1));
    memset(comp, 0, (size_t)(N + 1));
    seq_t r = 1;
    for (uint64_t p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= N; q += p) comp[q] = 1;
        uint64_t e = leg_a002295(N, p) - leg_a002295((uint64_t)n, p) - leg_a002295(M, p);
        while (e--) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
