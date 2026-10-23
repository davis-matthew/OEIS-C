/*
 * A168710  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168710: number of reduced words of length n in the Coxeter
 * group on 33 generators Sᵢ with (Sᵢ)² = (SᵢSⱼ)¹⁷ = I.
 *
 * The generating function is
 *   G(t) = (t¹⁷ + 2t¹⁶ + … + 2t + 1) /
 *          (496t¹⁷ – 31t¹⁶ – … – 31t).
 *
 * From this we obtain the linear recurrence
 *   a(0) = 1
 *   a(n) = 31·∑_{k=1}^{16} a(n–k) + 2            for 1 ≤ n ≤ 16
 *   a(17) = 31·∑_{k=1}^{16} a(17–k) – 495       ( = 33·32¹⁶ – 528 )
 *   a(n) = 31·∑_{k=1}^{16} a(n–k) – 496·a(n–17)  for n ≥ 18
 *
 * This implementation memoises the computed values in a static array
 * so that any call order of a(n) works in O(n) time per call.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (!cache) {
        /* initialise cache for indices 0 … 17 */
        cache_len = 18;                     /* 0 … 17 inclusive */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */

        cache[0] = (seq_t)1;

        /* a(1) … a(16) = 33 · 32^(k–1)  (computed by repeated multiplication) */
        seq_t val = (seq_t)33;
        cache[1] = val;
        for (idx_t k = 2; k <= 16; ++k) {
            val *= (seq_t)32;
            cache[k] = val;
        }

        /* sum of a(1) … a(16) */
        seq_t sum16 = (seq_t)0;
        for (idx_t k = 1; k <= 16; ++k) sum16 += cache[k];

        /* a(17) = 31 * sum16 – 495 */
        cache[17] = (seq_t)31 * sum16 - (seq_t)495;
    }

    if (n < cache_len) return cache[n];

    /* extend cache up to the required index */
    if (n >= cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= 16; ++j)
                sum += cache[i - j];
            cache[i] = (seq_t)31 * sum - (seq_t)496 * cache[i - 17];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
