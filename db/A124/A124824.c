/*
 * A124824  LambertW analog of the Bell numbers: a(n) = (1/e)*Sum_{k>=0}
 *          k*(n+k)^(n-1)/k! for n > 0 with a(0)=1.
 * https://oeis.org/A124824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = sum_{k=0}^{n} binom(n,k)*(n - k + 2)^(n - k)*a(k), with
 *            a(0)=1. This recurrence is equivalent to the OEIS definition and
 *            is used here to compute the sequence
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

static seq_t *a_cache = NULL;      /* memoization array */
static idx_t  a_cache_size = 0ULL; /* number of initialized elements */

/* Ensure the cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (a_cache_size > n) return;   /* already large enough */

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – leave cache as is */

    /* Initialize the new entries to 0. */
    for (idx_t i = a_cache_size; i <= n; ++i) tmp[i] = 0;

    a_cache = tmp;
    a_cache_size = new_size;

    /* The first term (n=0) is always 1. */
    if (a_cache_size > 0) a_cache[0] = 1;
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;      /* a(0) is defined to be 1 */

    ensure_cache(n);           /* allocate/extend the cache if necessary */

    /* Compute all values from the smallest missing index up to n. */
    for (idx_t i = a_cache[0] == 1 ? 1 : 0; i <= n; ++i)
    {
        if (i == 0) continue;          /* a(0) is already set */

        seq_t sum = 0;
        idx_t m = i - 1;               /* n in the recurrence formula */

        for (idx_t k = 0; k <= m; ++k)
        {
            seq_t bin  = binom(m, k);

            idx_t base_idx = m - k + 2;          /* (n - k + 2) */
            seq_t base      = (seq_t)base_idx;

            idx_t exp_idx  = m - k;              /* (n - k)  */
            seq_t pow       = big_pow(base, (seq_t)exp_idx);

            seq_t term = bin * pow;
            term = term * a_cache[k];
            sum += term;
        }

        a_cache[i] = sum;
    }

    return a_cache[n];
}
