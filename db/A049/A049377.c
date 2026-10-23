/*
 * A049377  Row sums of triangle A049352.
 * https://oeis.org/A049377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049377: Row sums of triangle A049352
   Recurrence derived from the exponential generating function.
   The values a(n) satisfy:
        a(0) = 1
        a(n+1) = sum_{r=0}^{n} a(r) * C(n-r+3,3) * (n! / r!)
   where C(k,3) = k*(k-1)*(k-2)/6.
   The implementation uses memoisation and computes terms in O(n^2) time
   and O(n) memory.  It works with any integer type seq_t provided by the
   build (8–128 bit or arbitrary precision). */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed a(n) values   */
static size_t cache_len = 0;       /* number of elements computed     */
static size_t cache_cap = 0;       /* current allocated capacity      */

/* Helper: binomial coefficient C(k,3) with k >= 3 */
static seq_t binom3(idx_t k)
{
    /* k*(k-1)*(k-2) is always divisible by 6 for integer k >= 3 */
    return (seq_t)k * (seq_t)(k-1) * (seq_t)(k-2) / 6;
}

/* Ensure the cache has space for indices up to 'need' (inclusive) */
static void ensure_cache_capacity(size_t need)
{
    if (need >= cache_cap) {
        cache_cap = need + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;      /* next index to compute is 1 */
    }

    size_t target = (size_t)n;
    if (target >= cache_len) {
        ensure_cache_capacity(target);
        /* Compute new terms up to a(target) */
        while (cache_len <= target) {
            /* Compute a[cache_len] where n = cache_len-1 in the formula */
            idx_t n_val = (idx_t)(cache_len - 1);

            seq_t sum = 0;
            seq_t ratio = 1;   /* ratio = n!/r! for current r */

            /* Iterate r from n_val down to 0 */
            for (size_t rr = n_val; ; --rr) {
                idx_t r = (idx_t)rr;
                idx_t k = n_val - r + 3;    /* k in binomial C(k,3) */
                seq_t coeff = binom3(k);

                seq_t term = cache[rr] * coeff;
                term *= ratio;
                sum += term;

                if (rr == 0) break;   /* finished loop */
                ratio *= (seq_t)rr;   /* update: ratio = ratio * r  */
            }
            cache[cache_len] = sum;
            cache_len++;
        }
    }
    return cache[target];
}
