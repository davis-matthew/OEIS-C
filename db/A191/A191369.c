/*
 * A191369  Numbers n with k divisors such that n-1 and n+1 in binary
 *          representation have same number k of 0's as 1's.
 * https://oeis.org/A191369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   balanced_with_k, cache_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Search for the numbers n such that
 *  τ(n)=k and the binary representations of n-1 and n+1 each contain
 *  k zeros and k ones.  The sequence is memoised so that any call
 *  to a(idx) returns the nth term (a(1) is the first listed term).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/balanced_with_k.h"
#include "../utils/cache_grow.h"

/*-------------  helper functions  ----------------------------------*/

/* number of divisors τ(n) for a 64‑bit integer */
static unsigned divisor_count_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    unsigned count = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            count *= (e + 1);
        }
    }
    if (m > 1) count *= 2;
    return count;
}

/*------------------  sequence function  ----------------------------*/

/* external linkage – required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index 0 is not defined */

    /* static state for memoisation */
    static uint64_t tested  = 1;        /* smallest integer already examined */
    static seq_t  *cache = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;

    /* grow the cache until we have at least n terms */
    while (len < n) {
        ++tested;

        unsigned k = divisor_count_uint64(tested);
        if (balanced_with_k(tested - 1, k) && balanced_with_k(tested + 1, k)) {
            if (len == cap) cache_grow(&cache, &cap);
            cache[len++] = (seq_t)tested;     /* cast is safe for all allowed seq_t */
        }
    }

    return cache[n - 1];   /* sequence is 1‑based */
}
