/*
 * A057659  Prime numbers whose square is composed of just two different
 *          decimal digits.
 * https://oeis.org/A057659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the n-th prime whose square consists of exactly two
 *            distinct decimal digits
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;   /* stored qualifying primes       */
static size_t cache_len   = 0;       /* number of cached primes        */
static size_t cache_cap   = 0;       /* allocated capacity of cache    */
static idx_t  last_checked = 1;      /* last integer examined for prime */

seq_t a(idx_t n)
{
    /* n must be positive; for n==0, return 0 (undefined) */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n items */
    while (cache_len < n) {
        last_checked++;

        /* Skip even numbers greater than 2 */
        if (last_checked > 2 && (last_checked & 1) == 0)
            continue;

        /* Prime test */
        if (!is_prime_big((seq_t)last_checked))
            continue;

        /* Square of the prime */
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;

        /* Count distinct digits of the square */
        unsigned char seen[10] = {0};
        size_t distinct = 0;
        seq_t tmp = sq;
        if (tmp == 0) {
            seen[0] = 1;
            distinct = 1;
        } else {
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (!seen[d]) {
                    seen[d] = 1;
                    ++distinct;
                    if (distinct > 2) break;
                }
                tmp /= 10;
            }
        }

        /* Accept if exactly two distinct digits */
        if (distinct == 2) {
            /* Ensure enough space in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp_buf = (seq_t *)realloc(prime_cache,
                                                   new_cap * sizeof(seq_t));
                if (!tmp_buf) abort();   /* allocation failure */
                prime_cache = tmp_buf;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* Return the cached prime (1‑based index) */
    return prime_cache[n - 1];
}
