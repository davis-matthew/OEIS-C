/*
 * A205804  E.g.f.: -log( Sum_{n>=0} (-x)^(n^2) / (n^2)! ).
 * https://oeis.org/A205804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -[x^n] log( 1 - x + x^4/4! - x^9/9! + … ) Recurrence
 *            (derived for exponential generating functions): a(1) = 1 for n ≥
 *            2 let S_minus = Σ_{m=1}^{⌊√(n-1)⌋} C(n-1, n-m²-1) * a(n-m²) (m
 *            odd → g=-1) let S_plus = Σ_{m=1}^{⌊√(n-1)⌋} C(n-1, n-m²-1) *
 *            a(n-m²) (m even → g=+1) val = S_minus - S_plus /...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"
#include "../utils/binom.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t capacity = 0;

    if (n == 0) return (seq_t)0;           // unused, but defined
    if (capacity <= n) {                   /* enlarge cache if needed   */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < capacity; ++i) new_cache[i] = cache[i];
        for (idx_t i = capacity; i <= n; ++i) new_cache[i] = 0;
        if (cache) seq_free(cache);
        cache = new_cache;
        capacity = (idx_t)new_cap;
    }

    /* initialise first term if not already */
    if (capacity == 1) {                   /* only cache[0] allocated   */
        cache[0] = (seq_t)0;               /* unused */
    }
    if (capacity > 1 && cache[1] == 0) {   /* first term a(1) */
        cache[1] = (seq_t)1;
    }

    for (idx_t i = 2; i <= n; ++i) {        /* compute a(i) if missing */
        if (cache[i] != 0) continue;        /* already computed   */
        seq_t s_minus = (seq_t)0;
        seq_t s_plus  = (seq_t)0;

        idx_t max_m = _floor_isqrt_u64(i - 1);
        for (idx_t m = 1; m <= max_m; ++m) {
            idx_t sq = m * m;
            idx_t j = i - sq;               /* j ≥ 1 */
            seq_t binc = binom(i - 1, j - 1);
            seq_t term = binc * cache[j];
            if (m & 1)        // odd → g = -1  → contributes to s_minus
                s_minus = s_minus + term;
            else              // even → g = +1  → contributes to s_plus
                s_plus = s_plus + term;
        }

        seq_t val = s_minus - s_plus;        /* s_minus ≥ s_plus always */

        /* adjust for perfect square of i */
        idx_t sq = _floor_isqrt_u64(i);
        if (sq * sq == i) {                  /* i is a perfect square */
            if ((sq & 1) == 0)                /* even sqrt → g = +1 → subtract 1 */
                val = val - 1;
            else                              /* odd sqrt → g = -1 → add 1  */
                val = val + 1;
        }

        cache[i] = val;
    }

    return cache[n];
}
