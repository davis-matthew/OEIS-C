/*
 * A029109  Expansion of 1/((1-x)(1-x^6)(1-x^7)(1-x^11)).
 * https://oeis.org/A029109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts 1, 6, 7, 11 (order
 *            of summands irrelevant). This is the coefficient of x^n in 1/((1
 *            - x)(1 - x^6)(1 - x^7)(1 - x^11))
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper for allocating an array of seq_t that works both in C and C++.
   In C++ we use placement new; in C we use calloc which zero‑initializes.
   The array is explicitly zero‑initialized afterwards to be safe with
   arbitrary‑precision types that may not default to zero. */
static seq_t *alloc_seq_array(size_t sz)
{
#if defined(__cplusplus)
    seq_t *p = new seq_t[sz];
    for (size_t i = 0; i < sz; ++i)
        p[i] = 0;
    return p;
#else
    seq_t *p = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!p)
        abort();          /* out of memory */
    return p;
#endif
}

/* Helper for freeing the array allocated by alloc_seq_array. */
static void free_seq_array(seq_t *p)
{
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

/* Main sequence function.  It maintains a static cache so that the
   function is O(1) after the first call for a given n.  Calls may be
   in any order; the cache is enlarged as needed. */
seq_t a(idx_t n)
{
    /* cached values and current capacity */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* allocated size of cache (in elements) */

    /* Ensure the cache can hold indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;

        /* Allocate new cache */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache)
            abort();

        /* Compute DP values into new_cache */
        const idx_t parts[4] = {1, 6, 7, 11};

        /* Zero‑initialise (already zeroed in alloc_seq_array) */
        new_cache[0] = 1;                       /* base case */

        for (int pi = 0; pi < 4; ++pi) {
            idx_t part = parts[pi];
            for (idx_t i = part; i < new_cap; ++i) {
                new_cache[i] += new_cache[i - part];
            }
        }

        /* Replace old cache (freeing it if necessary) */
        if (cache)
            free_seq_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[(size_t)n];
}
