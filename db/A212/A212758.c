/*
 * A212758  Number of (w,x,y,z) with all terms in {0,...,n} and w=[R/2], where
 *          R=max{w,x,y,z}-min{w,x,y,z} and [ ]=floor.
 * https://oeis.org/A212758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   compute_initials
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A212758
    Recurrence:
        a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - 3*a(n-6)
               + 3*a(n-7) + a(n-9) - a(n-10),
    with initial values
        a(0)=1, a(1)=8, a(2)=20, a(3)=57, a(4)=118, a(5)=172,
        a(6)=299, a(7)=468, a(8)=594, a(9)=865, a(10)=1196.
    The function computes the first 11 terms by brute‑force enumeration
    and then uses the recurrence for all larger indices, memoising
    results for arbitrary‑order calls.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_initials.h"

/* Ensure that *ptr points to an array of at least 'needed' elements.
   The array is grown by successive doubling to keep amortised
   linear behaviour.  The allocation size is in terms of seq_t. */
static void
ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main sequence function.  The static data structures preserve results
   between calls, so calls may arrive in any order. */
seq_t a(idx_t n)
{
    /* Persistent cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t computed = 0;   /* number of values currently stored */
    static bool initialised   = false;

    /* At least 11 entries are required to hold the initial terms. */
    size_t need = (size_t)n + 1;
    if (need < 11) need = 11;
    ensure_capacity(need, &cap, &cache);

    /* Compute the first 11 terms once. */
    if (!initialised) {
        compute_initials(cache, 10);
        computed = 11;           /* indices 0…10 are now valid */
        initialised = true;
    }

    /* Return cached value if already computed. */
    if ((size_t)n < computed)
        return cache[n];

    /* Extend the cache up to index n using the recurrence. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t v = cache[i-1];
        if (i >= 3) v += (seq_t)3 * cache[i-3];
        if (i >= 4) v -= (seq_t)3 * cache[i-4];
        if (i >= 6) v -= (seq_t)3 * cache[i-6];
        if (i >= 7) v += (seq_t)3 * cache[i-7];
        if (i >= 9) v += cache[i-9];
        if (i >=10) v -= cache[i-10];
        cache[i] = v;
    }
    computed = (size_t)n + 1;
    return cache[n];
}
