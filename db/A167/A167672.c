/*
 * A167672  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) has the generating function
 *    (t^15 + 2*t^14 + … + 2*t + 1) / (105*t^15 - 14*t^14 - … - 14*t + 1).
 * From this one obtains the recurrence
 *    a(n) = 14*(a(n-1) + … + a(n-14)) - 105*a(n-15) + b(n)
 * where
 *    b(0) = 1,  b(1..14) = 2,  b(15) = 1,  b(n) = 0 for n>15.
 * (All indices <0 are treated as 0.)                                      */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* dynamic cache of computed terms   */
static idx_t  cache_size = 0;      /* number of elements stored (next free index) */

/* Ensure cache has space for terms up to n (≧ cache_size) and compute new terms */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;   /* already enough */

    /* enlarge allocation */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) { /* in practice this should never happen for the test ranges */
        /* aborting silently: return with cache unchanged (caller will see 0) */
        return;
    }
    cache = tmp;

    /* compute all new terms i = cache_size .. n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* 14 * a(i-k) for k = 1..14 */
        for (idx_t k = 1; k <= 14 && i >= k; ++k) {
            sum += (seq_t)14 * cache[i - k];
        }
        /* -105 * a(i-15) if i >= 15 */
        if (i >= 15) {
            sum -= (seq_t)105 * cache[i - 15];
        }
        /* add b(i) contribution */
        seq_t b = 0;
        if (i == 0 || i == 15) {
            b = 1;
        } else if (i <= 14) {
            b = 2;
        }
        sum += b;
        cache[i] = sum;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_size) return cache[n];
    /* if allocation failed, return zero */
    seq_t zero = 0;
    return zero;
}
