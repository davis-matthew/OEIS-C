/*
 * A142528  Primes congruent to 45 mod 52.
 * https://oeis.org/A142528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 45 (mod 52), offset 1
 * Range:     u8 n <= 2, u16 n <= 215, u32 n <= 216, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t cur_k = 1;          /* parameter k in 52*k + 45, starts at 1 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; undefined for n=0 */

    /* Ensure cache has at least n entries */
    while (cache_size < (size_t)n) {
        /* Compute candidate prime */
        uint64_t cand64 = 52ULL * cur_k + 45ULL;
        seq_t  cand = (seq_t)cand64;

        if (cand64 >= 2 && is_prime_big(cand)) {
            /* Grow cache if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        cur_k++;
    }
    return cache[n - 1];
}
