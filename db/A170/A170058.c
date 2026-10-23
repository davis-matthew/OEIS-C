/*
 * A170058  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 49, a(2) = 2352, … Recurrence (derived from the
 *            rational generating function): a(n) = (n==0 || n==36 ? 1 : (n>=1
 *            && n<=35 ? 2 : 0)) + 47*∑_{k=1}^{35} a(n-k) – 1128*a(n-36)
 *            (a(m)=0 for m<0) This holds for every n ≥ 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small, numerator
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"
#include "../utils/numerator.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if ((idx_t)cache_len > n)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t num = numerator(i);

        /* Sum of the previous 35 terms */
        seq_t s = 0;
        for (idx_t k = 1; k <= 35 && k <= i; ++k)
            s += cache[i - k];

        /* term = num + 47*s - 1128*a(i-36) (if applicable) */
        seq_t term = num;
        term += mul_small(s, 47);
        if (i >= 36)
            term -= mul_small(cache[i - 36], 1128);

        cache[i] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
