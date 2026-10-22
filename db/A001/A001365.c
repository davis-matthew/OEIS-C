/*
 * A001365  Expansion of
 *          1/(1-x)^2/(1-x^2)/(1-x^6)/(1-x^12)/(1-x^24)/(1-x^48)/(1-x^60).
 * https://oeis.org/A001365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)^2(1-x^2)) *
 *            1/((1-x^6)(1-x^12)(1-x^24)(1-x^48)(1-x^60)), so a(n) =
 *            Sum_{r=0..floor(n/6)} g(r) * f(n-6r), where f(m) =
 *            (floor(m/2)+1)*(m+1-floor(m/2)) and g(r) = number of partitions
 *            of r into parts {1,2,4,8,10}
 * Range:     u8 n <= 20, u16 n <= 97, u32 n <= 728, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001365_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/a001365_f.h"

seq_t a(idx_t n) {
    idx_t R = n / 6;
    uint64_t *g = (uint64_t *)calloc((size_t)R + 1, sizeof(uint64_t));
    static const idx_t parts[5] = {1, 2, 4, 8, 10};
    int p;
    idx_t r;
    seq_t s = 0;
    g[0] = 1;
    for (p = 0; p < 5; p++) {
        idx_t k = parts[p];
        for (r = k; r <= R; r++) g[r] += g[r - k];
    }
    for (r = 0; r <= R; r++) {
        s += (seq_t)g[r] * a001365_f(n - 6 * r);
    }
    free(g);
    return s;
}
