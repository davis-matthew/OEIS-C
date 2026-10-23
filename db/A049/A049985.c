/*
 * A049985  Primes of form k! - (k-1)! - 1.
 * https://oeis.org/A049985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form k! - (k-1)! - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   grow_seq, is_prime_big, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;          /* array of computed primes */
static size_t cache_cap = 0;         /* capacity of the cache */
static idx_t cache_len = 0;          /* number of primes stored */

static idx_t last_k = 1;             /* k for which fact_cur = k! */
static seq_t fact_cur = (seq_t)1;    /* k!  (value of factorial for last_k) */
static seq_t fact_prev = (seq_t)1;   /* (k-1)! for the next iteration */

static void compute_upto(idx_t n)
{
    while (cache_len < n)
    {
        /* advance to next k */
        ++last_k;

        /* prev factorial is current factorial before multiplication */
        fact_prev = fact_cur;

        /* fact_cur = fact_prev * last_k = last_k! */
        fact_cur *= (seq_t)last_k;

        /* term = k! - (k-1)! - 1 */
        seq_t term = fact_cur - fact_prev - (seq_t)1;

        /* if term is prime, store it */
        if (is_prime_big(term))
        {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = term;
        }
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
    }

    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, but return 0 */

    compute_upto(n);
    return cache[n - 1];
}
