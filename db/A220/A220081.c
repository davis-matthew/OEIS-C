/*
 * A220081  Primes of the form 15*k^2 - 15*k + 17.
 * https://oeis.org/A220081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 15*k^2 - 15*k + 17, for the smallest
 *            non‑negative integers k that produce a prime. The sequence skips
 *            duplicate values (k=1 gives the same prime 17 as k=0)
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;    /* cached prime values */
static size_t cache_cap = 0;   /* current cache capacity          */
static idx_t  cache_size = 0;  /* number of values cached          */
static idx_t  next_k   = 0;    /* next k to test                  */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0 */

    while (cache_size < n) {
        /* compute f(k) = 15*k^2 - 15*k + 17 */
        seq_t val = (seq_t)next_k;
        val = val * val;                    /* k^2      */
        val = val * (seq_t)15;              /* 15*k^2   */
        val -= (seq_t)15 * (seq_t)next_k;    /* -15*k    */
        val += (seq_t)17;                   /* +17      */

        if (is_prime_big(val)) {
            /* skip duplicates (happens for k=1) */
            if (cache_size == 0 || val != cache[cache_size - 1]) {
                if (cache_size >= cache_cap)
                    grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = val;
            }
        }
        ++next_k;
    }
    return cache[n - 1];
}
