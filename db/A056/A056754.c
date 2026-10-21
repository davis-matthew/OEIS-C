/*
 * A056754  Numbers k such that k | 9^k + 8^k + 7^k + 6^k + 5^k + 4^k + 3^k +
 *          2^k + 1^k.
 * https://oeis.org/A056754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A056754
 * Numbers k such that k | 9^k + 8^k + … + 1^k.
 * The function searches for k in ascending order, stores every
 * qualifying value in a static cache, and returns the (n‑1)-th
 * element of that cache.  This guarantees correct results for
 * any order of calls and for arbitrary-precision seq_t. */

#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;      /* cache of found terms        */
static size_t cache_cnt = 0;     /* number of terms cached      */
static size_t cache_cap = 0;     /* current cache capacity      */
static idx_t last_checked = 0;   /* largest k that has been examined */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    while (cache_cnt < n) {
        /* Examine next integer k */
        ++last_checked;
        seq_t k_mod = (seq_t)last_checked;
        seq_t sum = 0;

        /* Compute sum_{i=1}^9 i^k (mod k) */
        for (int base = 1; base <= 9; ++base) {
            seq_t p = mod_pow((seq_t)base, (uint64_t)last_checked, k_mod);
            sum = (sum + p) % k_mod;
        }

        /* If k divides the sum, store it */
        if (sum == 0) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = k_mod;
        }
    }

    return cache[n - 1];
}
