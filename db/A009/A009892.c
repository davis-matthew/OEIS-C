/*
 * A009892  Coordination sequence T3 for Zeolite Code RTE.
 * https://oeis.org/A009892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 174, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence derived from the
 * rational generating function of A009892.  With the denominator
 * polynomial
 *
 *     P(x) = (1 - x)^3*(1 + x^2)*(1 + x + x^2)*(1 + x + x^2 + x^3 + x^4)*
 *            (1 + x + x^2 + x^3 + x^4 + x^5 + x^6)
 *
 * and the numerator
 *
 *     N(x) = (1 + x)*(1 + 3x + 9x² + 15x³ + 27x⁴ + 38x⁵ + 48x⁶ + 56x⁷ +
 *            57x⁸ + 56x⁹ + 48x¹⁰ + 38x¹¹ + 27x¹² + 15x¹³ + 9x¹⁴ + 3x¹⁵ + x¹⁶),
 *
 * the sequence obeys
 *
 *     a(n) + 0·a(n‑1) + a(n‑2) - a(n‑3) + 0·a(n‑4) - 2·a(n‑5) + 0·a(n‑6)
 *     - 2·a(n‑7) + a(n‑8) - a(n‑9) + 2·a(n‑10) + 0·a(n‑11) + 2·a(n‑12)
 *     + 0·a(n‑13) + a(n‑14) - a(n‑15) + 0·a(n‑16) - a(n‑17) = qₙ,
 *
 * where qₙ is the coefficient of N(x) at power n (qₙ = 0 for n > 17).
 * The first seventeen terms (qₙ for n ≤ 17) supply the initial values.
 *
 * The implementation memoises the sequence in a static cache and extends
 * the cache only when required.  It works for all allowed builds
 * (fixed‑size or arbitrary‑precision) and runs in O(n) time per call.
 */

#include <stdlib.h>
#include <stdint.h>

static const int coeff[18] = {
    1, 0, 1, -1, 0, -2, 0,
    -2, 1, -1, 2, 0, 2, 0,
    1, -1, 0, -1
};

static const int ncoeff[18] = {
    1, 4, 12, 24, 42, 65, 86,
    104, 113, 113, 104, 86, 65,
    42, 24, 12, 4, 1
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        for (idx_t m = cache_len; m <= n; ++m) {
            seq_t rhs = (m <= 17) ? (seq_t)ncoeff[(int)m] : 0;
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int i = 1; i < 18; ++i) {
                if (m < (idx_t)i) continue;
                seq_t prev = cache[m - (idx_t)i];
                int c = coeff[i];
                if (c > 0) {
                    sum_pos += prev * (seq_t)c;
                } else if (c < 0) {
                    sum_neg += prev * (seq_t)(-c);
                }
            }
            seq_t val = rhs + sum_neg;
            val -= sum_pos;
            cache[m] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
