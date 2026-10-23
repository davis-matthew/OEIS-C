/*
 * A144980  Natural numbers k such that k+1 is divisible by the sum of the
 *          decimal digits of k.
 * https://oeis.org/A144980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th natural number k such that (k+1) is divisible
   by the sum of the decimal digits of k.  The sequence is A144980. */
#include <stdlib.h>

/* Static cache of found terms.  cache[i] == a(i+1) */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static seq_t next_k = 1;       /* next integer to test */

/* Helper: expand cache capacity if needed. */
static void grow_cache(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Find and store terms until we have at least target terms in cache. */
static void ensure_cache(idx_t target)
{
    size_t t = (size_t)target;
    while (cache_len < t) {
        grow_cache();
        /* Search for the next admissible k starting from next_k. */
        seq_t k = next_k;
        while (1) {
            /* Compute sum of decimal digits of k. */
            seq_t sum = 0;
            seq_t tmp = k;
            while (tmp > 0) {
                sum += tmp % 10;
                tmp /= 10;
            }
            if (sum == 0) sum = 1;      /* safety, k>=1 so sum>=1 */

            if (((k + 1) % sum) == 0) {
                cache[cache_len++] = k;
                next_k = k + 1;        /* start next search after k */
                break;
            }
            ++k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* offset starts at 1; behaviour for n<1 is irrelevant */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
