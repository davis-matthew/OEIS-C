/*
 * A212525  Primes containing a digit 3.
 * https://oeis.org/A212525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primes containing decimal digit 3 (OEIS A212525)
 * Range:     u8 n <= 20, u16 n <= 3528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_digit3_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/contains_digit3_seq.h"

/* Cached terms – up to a fixed maximum size */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;          /* number of terms stored */
static idx_t last_checked = 2;       /* last integer examined */

/* Return the nth prime containing a digit 3 (a(1)=3). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0; guard */

    /* If requested term is already cached, return it directly. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Extend the cache until it contains the required term,
       or until the maximum cache size is reached. */
    while (cache_len < n && cache_len < MAX_CACHE)
    {
        /* Find the next prime. */
        do
        {
            last_checked++;
        } while (!is_prime(last_checked));

        /* Accept the prime only if it contains a digit 3. */
        seq_t candidate = (seq_t)last_checked;
        if (contains_digit3_seq(candidate))
            cache[cache_len++] = candidate;
    }

    /* If the term is now cached, return it. */
    if (cache_len >= n)
        return cache[n - 1];

    /* For n larger than the cache size, continue searching
       without caching further terms. */
    idx_t count = cache_len;
    while (count < n)
    {
        /* Continue scanning from the last checked number. */
        do
        {
            last_checked++;
        } while (!is_prime(last_checked));

        seq_t candidate = (seq_t)last_checked;
        if (contains_digit3_seq(candidate))
        {
            count++;
            if (count == n)
                return candidate; /* found the requested term */
        }
    }

    /* Should never reach here. */
    return 0;
}
