/*
 * A169560  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 19, a(2) = 342, ... Recurrence: for n ≥ 0 a(n)
 *            = N(n) + 17 * Σ_{k=1}^{34} a(n-k) – 153 * a(n-35), where N(n) =
 *            1 if n = 0 or 35, 2 if 1 ≤ n ≤ 34, 0 otherwise. Negative indices
 *            are treated as 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;           /* array of computed terms */
static size_t cache_len = 0;          /* number of terms currently stored */
static size_t cache_cap = 0;          /* allocated capacity */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = n + 1;
    if (cache_cap >= required)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len)
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute N(i) */
        seq_t Ni;
        if (i == 0 || i == 35)
            Ni = 1;
        else if (i <= 34)
            Ni = 2;
        else
            Ni = 0;

        /* Sum of the previous 34 terms */
        seq_t sum34 = 0;
        idx_t kmax = (i < 34) ? i : 34;
        for (idx_t k = 1; k <= kmax; ++k)
            sum34 += cache[i - k];

        /* a(i) = Ni + 17*sum34 – 153*a(i-35) if i≥35 */
        seq_t ai = Ni + (seq_t)17 * sum34;
        if (i >= 35)
            ai -= (seq_t)153 * cache[i - 35];

        cache[i] = ai;
    }
    cache_len = n + 1;
    return cache[n];
}
