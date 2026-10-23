/*
 * A046673  a(n) = (2n)!*Sum_{i=1..n} 1/i.
 * https://oeis.org/A046673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * sum_{i=1..n} 1/i
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* (2k)!  for k = 0..n */
static seq_t *a_cache   = NULL;   /* a(k)    for k = 0..n */
static idx_t  cache_size = 0;     /* allocated size (>= cached_max+1) */
static idx_t  cached_max = 0;     /* highest k for which values are filled */

/* ensure that caches contain values up to k = n (inclusive)   */
static void ensure_up_to(idx_t n)
{
    if (n <= cached_max)            /* nothing to do */
        return;

    /* Allocate (or grow) storage to hold indices 0..n */
    size_t new_len = (size_t)(n + 1);
    if (new_len > cache_size) {
        fact_cache = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        a_cache   = (seq_t *)realloc(a_cache,   new_len * sizeof(seq_t));
        cache_size = new_len;
    }

    /* initialize the factorial of 0 ( (2*0)! = 1 ) */
    if (cached_max == 0) {
        fact_cache[0] = (seq_t)1;
        a_cache[0]    = (seq_t)0;   /* not used for the offset 1 sequence */
    }

    /* compute values from the next missing index up to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t prev_fact = fact_cache[k - 1];

        /* multipliers (2k-1) and (2k) computed in the idx_t domain,
           then cast to seq_t for the big‑integer operations. */
        seq_t mul1 = (seq_t)(2 * k - 1);
        seq_t mul2 = (seq_t)(2 * k);

        /* (2k)!  = (2k-2)! * (2k-1) * (2k) */
        fact_cache[k] = prev_fact * mul1 * mul2;

        /* a(k) = (2k)(2k-1) * a(k-1) + ((2k)!)/k  */
        seq_t part1 = a_cache[k - 1] * mul1 * mul2;   /* (2k)(2k-1) * a(k-1) */
        seq_t part2 = fact_cache[k] / k;             /* fact(k) / k   */
        a_cache[k]  = part1 + part2;
    }

    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)        /* behavior below the offset is irrelevant */
        return (seq_t)0;

    ensure_up_to(n);
    return a_cache[n];
}
