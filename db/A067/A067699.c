/*
 * A067699  Number of comparisons made in a version of the sorting algorithm
 *          QuickSort for an array of size n with n identical elements.
 * https://oeis.org/A067699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil((n+1)/2) + a(ceil(n/2)) + a(floor(n/2)) a(1) = 0,
 *            a(2) = 4, a(3) = 8
 * Range:     u8 n <= 37, u16 n <= 4728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;    /* highest n for which cache[n] is valid */

static void ensure_cache(idx_t n)
{
    if (cache_max >= n) return;

    /* Need capacity for indices 0..n (0 unused) */
    size_t new_cap = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (cache == NULL) {
        /* allocation failed – in contest environment this should not happen */
        abort();
    }

    /* Fill missing entries */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 0;
        } else if (i == 2) {
            cache[2] = 4;
        } else if (i == 3) {
            cache[3] = 8;
        } else {
            /* compute recurrence */
            idx_t floor_half = i / 2;
            idx_t ceil_half  = (i + 1) / 2;
            seq_t comp_part = 2 * ((seq_t)((i + 2) / 2));        /* 2*ceil((i+1)/2) */
            seq_t val = comp_part + cache[ceil_half] + cache[floor_half];
            cache[i] = val;
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1; return 0 safely */
    ensure_cache(n);
    return cache[n];
}
