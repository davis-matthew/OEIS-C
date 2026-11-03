/*
 * A065156  Numbers n such that some Lucas number (A000204) is divisible by n.
 * https://oeis.org/A065156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Composite set of integers that divide some Lucas number * (OEIS
 *            A065156)
 * Range:     u8 n <= 80, u16 n <= 10780, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 78 terms through n = 9731 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests each positive integer n in turn.  For a given
 * n it computes the Lucas sequence modulo n until a zero appears
 * (which proves that n divides some Lucas number) or until the
 * index exceeds n (the OEIS theorem guarantees that if n divides
 * any Lucas number, a divisor appears by index n).  All such n are
 * collected in increasing order; the function a(n) returns the nth
 * element of this list.  The method is O(m^2) in the worst case
 * where m is the largest integer examined, which is comfortably
 * below the limit (≈20000) needed for the first 10000 terms.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether m divides some Lucas number L_k with k ≤ m */
static bool divides_some_lucas(idx_t m)
{
    if (m == 0)
        return false;                     /* not used, but keep safe */

    /* L_0 = 2 , L_1 = 1 */
    if (2 % m == 0 || 1 % m == 0)
        return true;

    idx_t prev = 2 % m;
    idx_t curr = 1 % m;

    for (idx_t i = 2; i <= m; ++i) {
        idx_t next = prev + curr;
        if (next >= m)
            next -= m;
        if (next == 0)
            return true;
        prev = curr;
        curr = next;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cache_size = 0; /* number of terms stored       */
    static size_t cache_cap   = 0; /* current allocated capacity  */
    static idx_t next_int = 1;     /* next integer to test         */

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Expand cache until we have at least n terms */
    while (cache_size < n) {
        if (divides_some_lucas(next_int)) {
            if (cache_size == cache_cap) {      /* grow the cache   */
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = (seq_t)next_int;
        }
        ++next_int;
    }

    return cache[(size_t)(n - 1)];
}
