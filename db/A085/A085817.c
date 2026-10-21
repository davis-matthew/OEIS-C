/*
 * A085817  Start at (2n+1)/4 and iterate the map x -> x*ceiling(x); sequence
 *          gives values of n for which the denominators in the orbit drop
 *          directly from 4 to 1, bypassing 2.
 * https://oeis.org/A085817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A085817.
   For each integer k ≥ 1, the map
      x -> x * ceil(x)
   is iterated starting from (2k+1)/4.
   The orbit is considered successful if the denominator
   falls directly from 4 to 1 (i.e. a factor 4 is cancelled)
   without ever encountering a denominator equal to 2.
   The implementation keeps track of the current numerator
   modulo 1024 to decide the next step and detects loops
   that never succeed.  Results are memoised. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* determine whether k belongs to A085817 */
static bool good(idx_t k)
{
    /* current numerator modulo 1024 (the denominator always stays 4 or 1) */
    uint32_t m = (uint32_t)(((2ULL * (uint64_t)k + 1ULL) & 0x3FFULL)); /* (2k+1) mod 1024 */

    bool visited[1024] = {0};

    while (1) {
        if (visited[m])               /* cycle detected, no success */
            return false;
        visited[m] = true;

        /* ceil(x) where x = m / 4  (x is rational)
           ceil(x) = floor((m + 3)/4)   (since denominator is 4) */
        uint32_t c = (m + 3U) >> 2U;      /* floor((m+3)/4)  an integer ≤255 */
        uint32_t c_mod4 = c & 3U;          /* c mod 4 */

        if (c_mod4 == 2U)      /* denominator would become 2 -> fail */
            return false;
        if (c_mod4 == 0U)      /* denominator becomes 1 -> success */
            return true;

        /* update numerator: x' = x * ceil(x)  ; keep it modulo 1024 */
        m = (m * c) & 0x3FFU;
    }
}

/* sequence a(n) with offset 1.  Results are memoised. */
seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;
    static idx_t cache_sz = 0;
    static idx_t cache_cap = 0;
    static idx_t candidate = 1;          /* first integer to test */

    if (n < 1)
        return 0;                       /* behaviour for n<1 is undefined */

    /* generate entries until we have at least 'n' of them */
    while (cache_sz < n) {
        /* test current candidate */
        if (good(candidate)) {
            /* ensure capacity */
            if (cache_sz == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
