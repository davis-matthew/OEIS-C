/*
 * A168933  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(0) = 1
 *   1 ≤ n ≤ 21 : a(n) = 16 * 15^(n-1)
 *   n ≥ 22     : a(n) = 14 * Σ_{k=1}^{21} a(n-k) – 105 * a(n-22)
 * (Indices are 0‑based)
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;     /* cache of computed terms          */
static size_t cache_size = 0;   /* number of terms currently stored */

/* enlarge cache so that a(n) is available */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n)
        return;  /* already computed */

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();   /* fatal memory error */

    /* compute new terms */
    for (size_t i = cache_size; i < new_size; ++i)
    {
        if (i == 0)                    /* a(0) = 1 */
            new_cache[i] = 1;
        else if (i <= 21)              /* 1 ≤ n ≤ 21 : 16*15^(n‑1) */
        {
            seq_t val = 1;
            for (size_t j = 0; j < i-1; ++j)
                val *= 15;
            val *= 16;
            new_cache[i] = val;
        }
        else                            /* n ≥ 22 : linear recurrence of order 22 */
        {
            seq_t sum = 0;
            for (size_t j = 1; j <= 21; ++j)
                sum += new_cache[i-j];
            new_cache[i] = sum * 14 - new_cache[i-22] * 105;
        }
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
