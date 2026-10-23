/*
 * A169099  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For n >= 1: * a(n) = 36 * (a(n-1)+a(n-2)+…+a(n-24))
 *            * - 666 * a(n-25) (if n >= 25) * + b_n * where * b_0 = 1 * b_n =
 *            2 for 1 <= n <= 24 * b_25 = 1 * b_n = 0 for n >= 26 * * This
 *            recurrence, derived from the generating function * G(t) = (1 +
 *            2t + … + 2t^24 + t^25) / * (1 - 36t - … - 3...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic construction. */
    static std::vector<seq_t> cache;
    size_t old_len = cache.size();
    if (old_len <= (size_t)n) {
        cache.resize((size_t)n + 1);
        for (idx_t k = (idx_t)old_len; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 24; ++i)
                if (k >= i)
                    sum += cache[k - i];
            seq_t term = sum * 36;
            if (k >= 25)
                term -= cache[k - 25] * 666;
            /* add b_n */
            if (k == 0)
                term += 1;
            else if (k <= 24)
                term += 2;
            else if (k == 25)
                term += 1;
            cache[k] = term;
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc.  seq_t is assumed to be a
       scalar type in the C build, so this is safe. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 24; ++i)
                if (k >= i)
                    sum += cache[k - i];
            seq_t term = sum * 36;
            if (k >= 25)
                term -= cache[k - 25] * 666;
            /* add b_n */
            if (k == 0)
                term += 1;
            else if (k <= 24)
                term += 2;
            else if (k == 25)
                term += 1;
            cache[k] = term;
        }
        cache_len = n + 1;
    }
    return cache[n];
#endif
}
