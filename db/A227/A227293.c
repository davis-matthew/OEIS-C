/*
 * A227293  G.f.: sqrt( Sum_{n>=0} 4^n*x^(n^2) ).
 * https://oeis.org/A227293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(0) = 1 * for n ≥ 1: * let s = n if n is a perfect square;
 *            otherwise s = 0. * let f = 4^s (f=0 when s=0) * a(n) = (f -
 *            Σ_{i=1}^{n-1} a(i)*a(n-i)) / 2 * * // This follows from A(x)^2 =
 *            Σ_{k≥0} 4^k x^{k^2}, * // where A(x)=∑_{n≥0} a(n)x^n with a(0)=1
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-03)
 * Helpers:   ipow, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of computed terms          */
    static idx_t  cache_len = 0;     /* number of cached terms returned */

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) abort();          /* out of memory */
        cache = tmp;

        for (idx_t k = cache_len; k < new_len; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* Σ_{i=1}^{k-1} a(i)*a(k-i) */
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i < k; ++i) {
                sum += cache[i] * cache[k - i];
            }

            /* f(k): 4^t if k = t^2, else 0 */
            seq_t f = (seq_t)0;
            idx_t r = isqrt(k);
            if (r * r == k) {
                f = ipow((seq_t)4, (seq_t)r);
            }

            cache[k] = (f - sum) / (seq_t)2;
        }

        cache_len = new_len;
    }

    return cache[n];
}
