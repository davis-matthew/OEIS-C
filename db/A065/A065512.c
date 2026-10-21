/*
 * A065512  Numbers k such that sigma(k) + 1 is prime.
 * https://oeis.org/A065512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the n-th positive integer k such that sigma(k) + 1 is
 *            prime
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache */
    static size_t len   = 0;   /* number of entries currently stored */
    static idx_t  next_k   = 1; /* next k to test */
    static idx_t  found    = 0; /* number of qualifying k's found so far */

    if (n == 0)
        return 0;                /* undefined for n < 1, return 0 */

    /* compute until we have at least n entries in the cache */
    while (found < n) {
        seq_t sigma = sigma_of(next_k);
        seq_t s = sigma + (seq_t)1;
        if (is_prime_big(s)) {
            /* grow the array if needed */
            if (len == cap)
                grow_seq_array(&cache, &cap, len + 1);
            cache[len++] = (seq_t)next_k;
            ++found;
        }
        ++next_k;
    }

    return cache[(size_t)(n - 1)];
}
