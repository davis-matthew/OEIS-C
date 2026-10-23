/*
 * A154409  Primes of the form 10n^2+6n+1.
 * https://oeis.org/A154409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 10*n*n + 6*n + 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility headers for primality test and resizing an array */
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Sanity check: OEIS offset is 1; return 0 for n < 1 */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: primes found so far, next k to test, and array size */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of filled elements    */
    static idx_t next_k   = 1;     /* next k value to test (starts at 1) */

    /* Grow the cache until it contains the n-th element */
    while (cache_len < (size_t)n) {
        idx_t k = next_k++;
        seq_t val = (seq_t)10 * k * k + (seq_t)6 * k + (seq_t)1;
        if (is_prime_big(val)) {
            ensure_capacity((size_t)cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
