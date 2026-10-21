/*
 * A086929  a(1) = 1, a(n) = n times (the digit reversal of the (n-1)-th
 *          partial sum).
 * https://oeis.org/A086929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A086929:
   a(1) = 1,
   a(n) = n * rev( Σ_{k=1}^{n-1} a(k) )  for n > 1
   (rev = digit reversal in base 10) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/rev_decimal.h"

static seq_t *cache_terms = NULL;   /* a[k-1] for k>=1 */
static seq_t *cache_sums  = NULL;   /* partial sums S[k-1] = Σ_{i=1}^k a[i] */
static size_t cache_size  = 0;      /* number of cached elements */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Grow the cache arrays at least to 'needed' elements.  */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache_terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache_terms = tmp;

    tmp = (seq_t *)realloc(cache_sums,  new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache_sums = tmp;

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* not used; guard against bad input */

    /* compute all terms up to n */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 1;            /* a(1) = 1 */
        } else {
            seq_t prev_sum = cache_sums[cache_size - 1];
            seq_t rev = rev_decimal(prev_sum);
            val = ((seq_t)k) * rev;   /* n * rev(previous partial sum) */
        }

        /* ensure capacity before storing */
        if (cache_size >= cache_cap)
            ensure_cache_size(cache_size + 1);

        cache_terms[cache_size] = val;
        cache_sums[cache_size]  = (cache_size == 0) ? val
                                                    : cache_sums[cache_size - 1] + val;
        ++cache_size;
    }

    return cache_terms[(size_t)n - 1];
}
