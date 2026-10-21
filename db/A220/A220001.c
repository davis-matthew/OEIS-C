/*
 * A220001  Benes network size for permutations of n.
 * https://oeis.org/A220001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 5602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A220001):
   a(n) = 2*floor(n/2) + a(floor(n/2)) + a(ceil(n/2))   for n > 2
   with a(1) = 0, a(2) = 1.  */

/* The routine uses a small cache for repeated queries, but the
   calculation itself is purely recursive and works for any size
   of n that the type idx_t can hold.  */

#include <stddef.h>
#include <stdbool.h>

#define CACHE_LIMIT 10000            /* enough for the test range   */

/* static cache for small n to speed up repeated calls */
static seq_t  cache[CACHE_LIMIT + 1];
static bool   cached[CACHE_LIMIT + 1] = {false};

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* use cache when it is available */
    if (n <= CACHE_LIMIT) {
        if (cached[n]) return cache[n];
    }

    /* recursive evaluation */
    idx_t f = n >> 1;          /* floor(n/2)   */
    idx_t c = (n + 1) >> 1;    /* ceil (n/2)   */

    seq_t part = (seq_t)2 * (seq_t)f;       /* 2*floor(n/2) */
    seq_t val   = part + a(f) + a(c);

    /* store result in cache if within the cached range */
    if (n <= CACHE_LIMIT) {
        cache[n]   = val;
        cached[n]  = true;
    }

    return val;
}
