/*
 * A168851  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168851:
   a(0) = 1.
   For 1 ≤ n < 20: a(n) = 2 + 28 * Σ_{k=0}^{n-1} a(k).
   For n = 20: a(20) = 1 + 28 * Σ_{k=0}^{18} a(k) – 406 * a(0).
   For n > 20: a(n) = 28 * Σ_{k=1}^{19} a(n-k) – 406 * a(n-20).
   The recurrence is derived from the rational generating function
   (t²⁰+2t¹⁹+⋯+2t+1)/(406t²⁰−28t¹⁹−⋯−28t+1). */

#include <stdlib.h>
#include <stdbool.h>

/* Memoisation table for computed terms */
static seq_t *cache   = NULL;   /* array of terms, 0 … current_size-1 */
static size_t cache_cap = 0;    /* allocated capacity (in terms) */
static idx_t  cache_size = 0;   /* number of terms currently stored */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (cache && (idx_t)cache_cap > n)          /* already large enough */
        return;

    size_t needed = (size_t)n + 1;              /* terms 0 … n */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                               /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation; calls may be in any order. */
seq_t
a(idx_t n)
{
    ensure_capacity(n);

    /* compute any terms missing up to and including n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t val;
        if (i < 20)            /* 0 ≤ i ≤ 19 */
        {
            seq_t q = (i == 0) ? (seq_t)1 : (seq_t)2;
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];
            val = q + (seq_t)28 * sum;
        }
        else if (i == 20)      /* special case for i = 20 */
        {
            seq_t q = (seq_t)1;
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 19; ++k)
                sum += cache[i - k];
            val = q + (seq_t)28 * sum - (seq_t)406 * cache[0];
        }
        else                    /* i > 20 */
        {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 19; ++k)
                sum += cache[i - k];
            val = (seq_t)28 * sum - (seq_t)406 * cache[i - 20];
        }
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
