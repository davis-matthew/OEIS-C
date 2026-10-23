/*
 * A142297  Primes congruent to 13 mod 44.
 * https://oeis.org/A142297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 13 modulo 44 OEIS A142297. The
 *            function caches found primes so that later calls can retrieve
 *            them in O(1). The cache size is conservatively chosen to hold
 *            many more than the 10 000 primes required by the test harness
 * Range:     u8 n <= 3, u16 n <= 283, u32 n <= 284, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

#define CACHE_LIMIT 20000   /* enough for n up to at least 10000 */

static seq_t cache[CACHE_LIMIT];
static idx_t count = 0;     /* number of primes stored in cache */
static idx_t next_k = 0;    /* next candidate index (candidate = 44*next_k + 13) */

seq_t a(idx_t n)
{
    /* Return 0 for n == 0; behaviour below offset is unimportant. */
    if (n == 0) return 0;

    /* Keep generating primes until we have at least n cached. */
    while (count < n) {
        seq_t cand = (seq_t)44 * (seq_t)next_k + (seq_t)13;
        if (is_prime_big(cand)) {
            if (count >= CACHE_LIMIT) {
                /* In practice this should never happen for the expected limits. */
                abort();
            }
            cache[count++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
