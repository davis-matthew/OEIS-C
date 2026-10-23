/*
 * A187397  Expansion of -2*x^4 *(3*x^13 +2*x^12 +x^11 -6*x^10 -10*x^9 -6*x^8
 *          +x^7 +7*x^6 +5*x^5 -x^4 -8*x^3 -11*x^2 -8*x -5) / ((x -1)^4 *(x
 *          +1)^2 *(x^2 +1)^2 *(x^2 +x +1)^2).
 * https://oeis.org/A187397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A187397 is the coefficient sequence of the generating function *
 *            -2*x^4*(3*x^13+2*x^12+x^11-6*x^10-10*x^9-6*x^8+x^7+7*x^6+5*x^5-x^4-8*x^3-11*x^2-8*x-5)
 *            * divided by (x-1)^4*(x+1)^2*(x^2+1)^2*(x^2+x+1)^2. * It
 *            satisfies the linear recurrence * a(n) = p_n - Σ_{k=1}^{14} q_k
 *            * a(n-k) * where q_k are...
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int coeff[15] = {
    1,  0, 0, -2, -2, 0, 1, 4, 1, 0, -2, -2, 0, 0, 1
};

static const long long p_coeff[18] = {
    0,  0,  0,  0, 10, 16, 22, 16,  2, -10, -14, -2,
   12, 20, 12, -2, -4, -6
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i < need; ++i) {
        int64_t p = 0;
        if (i < 18) p = p_coeff[i];

        int64_t sum_pos = 0;
        int64_t sum_neg = 0;
        for (int k = 1; k <= 14; ++k) {
            if (i >= (size_t)k) {
                int ck = coeff[k];
                seq_t prev = cache[i - k];
                int64_t prod = (int64_t)ck * (int64_t)prev;
                if (ck >= 0)
                    sum_pos += prod;
                else
                    sum_neg -= prod;        /* prod is negative, so subtracting makes it positive */
            }
        }

        int64_t val = p - sum_pos + sum_neg;
        cache[i] = (seq_t)val;              /* val is non‑negative for the sequence */
    }
    cache_len = need;
    return cache[(size_t)n];
}
