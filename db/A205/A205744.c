/*
 * A205744  The sequence "u_{n-r}" used by Conway and Guy in the construction
 *          of A005318 and A096858.
 * https://oeis.org/A205744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005318(n - A002024(n)); A005318(0)=0, A005318(1)=1,
 *            A005318(i+1) = 2*A005318(i) - A005318(i - floor(1/2 + sqrt(2i)))
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* smallest k with k*(k+1) >= 2m (equals floor(1/2+sqrt(2m)) for m >= 1) */
static idx_t cg_round(idx_t m) {
    idx_t lo = 0, hi = 1;
    while (hi * (hi + 1) < 2 * m) hi *= 2;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) < 2 * m) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t k = cg_round(n);
    idx_t target = n - k;
    seq_t *c = mem_new(seq_t, (size_t)target + 2);
    c[0] = 0;
    c[1] = 1;
    for (idx_t i = 1; i < target; i++) {
        c[i + 1] = 2 * c[i] - c[i - cg_round(i)];
    }
    seq_t r = c[target];
    mem_free(c);
    return r;
}
