/*
 * A116616  Values of n such that prime(2n+1) mod 12 = 7.
 * https://oeis.org/A116616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that prime(2k+1) mod 12 = 7
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for offset 0 */

    /* Static state for memoisation */
    static idx_t   next_candidate = 0;   /* next k to test                     */
    static idx_t   cache_len      = 0;   /* number of terms cached             */
    static size_t  cache_cap      = 0;   /* current allocation size in terms    */
    static seq_t * cache          = NULL;/* array of cached terms               */

    /* Allocate cache once if needed.  A generous capacity guarantees
       that we never need to resize for n up to at least 10 000.   */
    if (cache == NULL) {
        cache_cap = 200000;                            /* safe upper bound */
        cache     = seq_alloc(cache_cap);
    }

    /* Generate terms until we have the requested index. */
    while (cache_len < n) {
        idx_t k = next_candidate;

        /* prime(2k+1) – the (2k+1)-th prime */
        uint32_t p = get_prime(2 * k + 1);

        /* Test the condition p mod 12 == 7. */
        if (p % 12 == 7) {
            cache[cache_len++] = (seq_t)k;
        }

        ++next_candidate;  /* move to next k */
    }

    /* a(n) is the (n‑1)th element of the cached list */
    return cache[n - 1];
}
