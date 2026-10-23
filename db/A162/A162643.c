/*
 * A162643  Numbers whose number of divisors is not a power of 2.
 * https://oeis.org/A162643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer m with τ(m) not a power of 2
 *            (τ(m) = number of divisors of m)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_capacity, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_power_of_two.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored sequence elements   */
    static size_t len   = 0;             /* current length of cache    */
    static size_t cap   = 0;             /* allocated capacity         */
    static uint64_t cur  = 1;            /* next integer to test       */

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        seq_t d = divisor_count_u64(cur);        /* τ(cur) */
        if (!is_power_of_two(d))                /* include if τ(cur) is NOT a power of two */
        {
            /* make sure the cache has room for the new element */
            ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[len++] = (seq_t)cur;          /* store cur as seq_t              */
        }
        cur++;                                 /* move to next integer            */
    }

    return cache[n - 1];
}
