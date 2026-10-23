/*
 * A009891  Coordination sequence T2 for Zeolite Code RTE.
 * https://oeis.org/A009891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1 + x) * (1 + 3*x + 8*x^2 + 17*x^3 + 26*x^4 + 38*x^5 +
 *            48*x^6 + 55*x^7 + 59*x^8 + 55*x^9 + 48*x^10 + 38*x^11 + 26*x^12
 *            + 17*x^13 + 8*x^14 + 3*x^15 + x^16) / ((1 - x)^3*(1 + x^2) * (1
 *            + x + x^2) * (1 + x + x^2 + x^3 + x^4) * (1 + x + x^2 + x^3 +
 *            x^4 + x^5 + x^6))
 * Range:     u8 n <= 10, u16 n <= 174, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n ≥ 18, a(n) = neg(n) - pos(n) where
   neg(n) collects terms with negative coefficients and
   pos(n) collects terms with positive coefficients according
   to the recurrence coefficients
   q = {1,0,1,-1,0,-2,0,-2,1,-1,2,0,2,0,1,-1,0,-1}  */
#include <stdlib.h>

static const int8_t q[18] = {1,0,1,-1,0,-2,0,-2,1,-1,2,0,2,0,1,-1,0,-1};
static const seq_t init[18] = {
    1,4,10,22,37,54,79,108,140,176,215,262,312,365,423,484,552,623
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Handle small indices directly */
    if (n < 18)
        return init[n];

    /* Determine required size */
    size_t need = (size_t)n + 1;

    /* Allocate or grow cache */
    if (cache_len < need) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure, return 0 */
        cache = tmp;
        /* Initialize first 18 terms if not already */
        if (cache_len < 18) {
            for (int i = 0; i < 18 && i < need; ++i)
                cache[i] = init[i];
            cache_len = (cache_len < 18) ? 18 : cache_len;
        }
        /* Compute remaining terms up to need-1 */
        for (size_t i = cache_len; i < need; ++i) {
            seq_t pos = 0, neg = 0, ai;
            for (int k = 1; k < 18; ++k) {
                ai = cache[i - k];
                int coeff = q[k];
                if (coeff > 0) {
                    pos += ai;
                    if (coeff == 2) pos += ai;
                } else if (coeff < 0) {
                    neg += ai;
                    if (coeff == -2) neg += ai;
                }
            }
            cache[i] = neg - pos;
        }
        cache_len = need;
    }
    return cache[n];
}
