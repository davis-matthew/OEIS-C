/*
 * A087245  Nonsquarefree abundant numbers.
 * https://oeis.org/A087245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth positive integer that is both abundant and nonsquarefree. */
#include <stddef.h>
#include "../utils/is_squarefree.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_realloc.h"

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
