/*
 * A002377  Least number of 4th powers needed to represent n.
 * https://oeis.org/A002377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = min_{k>=1, k^4<=n} a(n - k^4) + 1, a(0) = 0 (dynamic
 *            programming over n)
 * Range:     u8 n <= 9788, u16 n <= 9788, u32 n <= 9788, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *a002377_cache = NULL;
static uint64_t a002377_upto = 0;

seq_t a(idx_t n) {
    if (a002377_cache == NULL || n > a002377_upto) {
        uint64_t start = (a002377_cache == NULL) ? 1 : a002377_upto + 1;
        uint64_t lim = n;
        if (a002377_cache != NULL && lim < 2 * a002377_upto) lim = 2 * a002377_upto;
        if (lim < 64) lim = 64;
        unsigned char *p = (unsigned char *)realloc(a002377_cache, (size_t)(lim + 1));
        if (p == NULL) return (seq_t)0;
        a002377_cache = p;
        a002377_cache[0] = 0;
        for (uint64_t i = start; i <= lim; i++) {
            unsigned best = 255;
            for (uint64_t k = 1;; k++) {
                uint64_t q = k * k * k * k;
                if (q > i) break;
                unsigned v = (unsigned)a002377_cache[i - q] + 1u;
                if (v < best) best = v;
            }
            a002377_cache[i] = (unsigned char)best;
        }
        a002377_upto = lim;
    }
    return (seq_t)(unsigned)a002377_cache[n];
}
