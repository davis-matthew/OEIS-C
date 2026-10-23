/*
 * A136042  Base-2 MR-expansion of 1/29.
 * https://oeis.org/A136042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* MR expansion of 1/29.
   s(i) is the smallest exponent d such that (2^d)*p_i > 29,
   where p_1 = 1 and p_{i+1} = (2^d)*p_i – 29.
   The sequence is produced by the algorithm described in the OEIS
   entry and cached for fast indexed access. */

#include <stdlib.h>

/* static cache for the sequence values and the current numerators */
static seq_t *seq_cache = NULL;      /* memoised s(i) values              */
static seq_t *num_cache = NULL;      /* p_i values before each term       */
static idx_t  cache_size = 0;        /* number of terms currently cached  */
static idx_t  cache_cap   = 0;        /* allocated capacity for seq_cache   */
static idx_t  num_cap     = 0;        /* allocated capacity for num_cache   */
static const seq_t denom = 29;        /* denominator of the fraction       */

/* allocate or grow a cache of seq_t values */
static void grow_seq_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* allocate or grow a cache of numerator values */
static void grow_num_cache(void)
{
    size_t new_cap = (num_cap == 0) ? 17 : num_cap << 1;  /* start one larger than seq_cache */
    seq_t *tmp = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    num_cache = tmp;
    num_cap = (idx_t)new_cap;
}

/* compute the n-th term, with n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused, offset is 1 */

    /* initialise caches on first call */
    if (cache_cap == 0) {
        grow_seq_cache();          /* alloc 16 slots */
        grow_num_cache();          /* alloc 17 slots , num_cache[0] will be set */
        num_cache[0] = (seq_t)1;   /* first numerator p = 1 */
    }

    /* build cache up to requested index */
    while (cache_size < n) {
        /* if we are about to write out of bounds, grow the array first */
        if (cache_size >= cache_cap) grow_seq_cache();
        if (cache_size + 1 >= num_cap)  grow_num_cache();

        seq_t p = num_cache[cache_size];  /* current numerator p_i */
        seq_t tmp = p;
        idx_t d = 0;

        /* find smallest d with 2^d * p > denom */
        while (tmp <= denom) {
            tmp <<= 1;
            d++;
        }

        seq_cache[cache_size] = (seq_t)d;      /* produce s(i) = d */
        seq_t r = tmp - denom;                /* numerator for next step  */
        num_cache[cache_size + 1] = r;        /* store p_{i+1}          */

        cache_size++;     /* next index will be handled in next loop iteration */
    }

    return seq_cache[n - 1];  /* cache is zero‑based, n is 1‑based */
}
