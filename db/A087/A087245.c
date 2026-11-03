/*
 * A087245  Nonsquarefree abundant numbers.
 * https://oeis.org/A087245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9673 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth positive integer that is both abundant and nonsquarefree. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next = 1;

static void ensure_terms(idx_t target)
{
    while (cache_len < (size_t)target)
    {
        ++next;

        /* Skip numbers that are squarefree */
        if (!is_squarefree((seq_t)next))
        {
            seq_t sigma = sigma_of(next);
            seq_t two_m = (seq_t)next * 2;
            if (sigma > two_m)
            {
                /* Grow cache if necessary */
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1 */
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}
