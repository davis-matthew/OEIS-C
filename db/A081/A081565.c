/*
 * A081565  Binomial transform of expansion of exp(3cosh(x)).
 * https://oeis.org/A081565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n≥1: a(n) = a(n‑1) + 3 * Σ_{odd k=1}^{n‑1}
 *            binom(n‑1,k) * a(n‑1‑k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n + 1 > cap) {
        idx_t old_cap = cap;
        cap = n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));

        for (idx_t i = old_cap; i < cap; ++i) {
            if (i == 0) { cache[0] = (seq_t)1; continue; }
            if (i == 1) { cache[1] = (seq_t)1; continue; }

            idx_t k = i - 1;          /* use recurrence with n‑1 */
            seq_t sum = 0;
            seq_t bi  = (seq_t)k;     /* binom(k,1) */

            for (idx_t odd = 1; odd <= k; odd += 2) {
                sum += bi * cache[k - odd];
                if (odd + 2 <= k) {
                    bi = bi * (seq_t)(k - odd)      / (seq_t)(odd + 1);
                    bi = bi * (seq_t)(k - odd - 1)  / (seq_t)(odd + 2);
                }
            }
            cache[i] = cache[i-1] + (seq_t)3 * sum;
        }
    }
    return cache[n];
}
