/*
 * A214529  Expansion of f(x^29, -x^31) - x * f(x^19, -x^41) + x^3 * f(x^11,
 *          -x^49) - x^7 * f(-x, x^59) in powers of x where f() is Ramanujan's
 *          two-variable theta function.
 * https://oeis.org/A214529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in f(x^29, ‑x^31) – x f(x^19, ‑x^41) +
 *            x³ f(x^11, ‑x^49) – x⁷ f(‑x, x^59) where f(a,b) = Σ_{k∈ℤ}
 *            a^{k(k+1)/2} b^{k(k‑1)/2} (Ramanujan’s two‑variable theta
 *            function)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>
#else
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    if (cache.size() <= n) {
        /* grow to at least n+1, fill new entries with 0 */
        cache.resize(n + 1, 0);
        const idx_t sz = cache.size();

        /* bound for |k|: 30*k^2 <= sz-1  → |k| <= sqrt((sz-1)/30) + 2 */
        idx_t maxK = (idx_t)(std::sqrt((long double)(sz - 1) / 30.0L)) + 2;

        for (int64_t k = -(int64_t)maxK; k <= (int64_t)maxK; ++k) {
            /* parity‑based sign: (-1)^{k(k‑1)/2} equals +1 for k≡0,1 mod4, –1 for k≡2,3 mod4 */
            seq_t sign = ((k & 3) == 2 || (k & 3) == 3) ? -1 : 1;

            /* first term: 30k²‑k */
            idx_t e1 = 30*k*k - k;
            if (e1 < sz) cache[e1] += sign;

            /* second term: 30k²‑11k+1, with an overall minus */
            idx_t e2 = 30*k*k - 11*k + 1;
            if (e2 < sz) cache[e2] -= sign;

            /* third term: 30k²‑19k+3 */
            idx_t e3 = 30*k*k - 19*k + 3;
            if (e3 < sz) cache[e3] += sign;

            /* fourth term: 30k²‑29k+7, coefficient = –sign for even k, +sign for odd k */
            idx_t e4 = 30*k*k - 29*k + 7;
            seq_t coeff4 = -sign;
            if (k & 1) coeff4 = sign;   /* odd k */
            if (e4 < sz) cache[e4] += coeff4;
        }
    }
    return cache[n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) return 0;   /* allocation failed */
        cache = tmp;
        /* zero the whole new array */
        memset(cache, 0, new_cap * sizeof(seq_t));
        /* bound for |k| */
        long double bd = (long double)(new_cap - 1) / 30.0L;
        idx_t maxK = (idx_t)(sqrt((double)bd)) + 2;

        for (int64_t k = -(int64_t)maxK; k <= (int64_t)maxK; ++k) {
            seq_t sign = ((k & 3) == 2 || (k & 3) == 3) ? -1 : 1;

            idx_t e1 = 30*k*k - k;
            if (e1 < new_cap) cache[e1] += sign;

            idx_t e2 = 30*k*k - 11*k + 1;
            if (e2 < new_cap) cache[e2] -= sign;

            idx_t e3 = 30*k*k - 19*k + 3;
            if (e3 < new_cap) cache[e3] += sign;

            idx_t e4 = 30*k*k - 29*k + 7;
            seq_t coeff4 = -sign;
            if (k & 1) coeff4 = sign;   /* odd k */
            if (e4 < new_cap) cache[e4] += coeff4;
        }
        cap = new_cap;
    }
    return cache[n];
#endif
}
