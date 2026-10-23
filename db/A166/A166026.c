/*
 * A166026  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*(a(n-1)+a(n-2)+…+a(n-9)) - 406*a(n-10) * + b(n) where
 *            b(0)=1, b(1..9)=2, b(10)=1, else 0 (OEIS A166026)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored (cache_len==0 if none) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    /* seq_realloc understands the type of seq_t (built‑in or big) */
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n).  Valid for all n >= 0. */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;                /* a(0) = 1 */
            } else {
                seq_t sum = 0;

                /* b(i) contribution */
                if (i == 0 || i == 10) {
                    sum += 1;
                } else if (i >= 1 && i <= 9) {
                    sum += 2;
                }

                /* add 28 * a(i-k) for k = 1..9 */
                for (idx_t k = 1; k <= 9; ++k) {
                    if (i >= k)
                        sum += cache[i - k] * 28;
                }

                /* subtract 406 * a(i-10) if applicable */
                if (i >= 10)
                    sum -= cache[i - 10] * 406;

                val = sum;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
