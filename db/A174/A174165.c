/*
 * A174165  Numbers k for which (prime(k) - 1)^2 +1 is prime.
 * https://oeis.org/A174165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 18, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that (prime(k)-1)^2+1 is prime.
   a(n) returns the nth such k.  The function keeps a static cache
   of indices found so far. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *good = NULL;      /* cached good indices */
static size_t good_len = 0;      /* number of cached indices */
static size_t good_cap = 0;      /* allocated capacity of cached array */
static idx_t last_k = 0;         /* last prime index inspected */

/* Ensure that the cache can hold at least new_len elements. */
static void ensure_good_cap(size_t new_len)
{
    if (new_len <= good_cap)
        return;
    size_t new_cap = (good_cap == 0) ? 8 : good_cap * 2;
    while (new_cap < new_len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure – abort */
        abort();
    good = tmp;
    good_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, but safe value */

    /* Extend the cache until we have at least n elements. */
    while (good_len < (size_t)n) {
        ++last_k;                          /* next prime index to test */
        uint32_t p = get_prime(last_k);    /* kth prime (uint32_t fits many primes) */

        /* Compute (p-1)^2+1.  Use a wider type to avoid overflow when seq_t is small. */
        uint64_t prime_val = (uint64_t)p;
        uint64_t val64 = (prime_val - 1) * (prime_val - 1) + 1ULL;

        /* Cast to seq_t before primality test; the test assumes the value fits. */
        seq_t val = (seq_t)val64;

        if (is_prime_big(val)) {
            ensure_good_cap(good_len + 1);
            good[good_len++] = (seq_t)last_k;
        }
    }

    return good[n - 1];
}
