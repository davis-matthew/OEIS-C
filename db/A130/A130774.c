/*
 * A130774  Cumulative concatenation of A000204 Lucas numbers (beginning at
 *          1).
 * https://oeis.org/A130774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Concatenate(a(n-1),Lucas(n)) with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Lucas numbers are L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2).  The
 * sequence is built by concatenating each Lucas number to the previous
 * term.  This implementation caches all terms up to the maximal
 * requested index so that calls are O(1) after the first pass.  All
 * arithmetic is performed with the type seq_t and the index type
 * idx_t.  Memory allocation uses explicit casts so the code is valid
 * in both C11 and C++17. */

#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>   /* for abort() */
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Cached sequence values ----------------------------------------------*/
static seq_t *cache = NULL;        /* array of a(i) values          */
static size_t cache_cap = 0;       /* current allocation capacity  */
static idx_t  computed  = 0;       /* number of computed terms      */

/* Cached Lucas values ----------------------------------------------*/
static seq_t lucas_prev  = 0;  /* L_k  (current)                 */
static seq_t lucas_prev2 = 0;  /* L_{k-1} (previous)             */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but harmless */

    /* ---------------------------------------------------------------- */
    /* Initialise static data if this is the very first call           */
    /* ---------------------------------------------------------------- */
    if (computed == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();  /* out of memory */

        /* a(1) = 1, L(1) = 1, L(0) = 2                     */
        cache[0]     = 1;
        lucas_prev   = 1;        /* L1  */
        lucas_prev2  = 2;        /* L0  */
        computed = 1;
    }

    /* ---------------------------------------------------------------- */
    /* If the requested index is already computed, return it           */
    /* ---------------------------------------------------------------- */
    if (n <= computed)
        return cache[n - 1];

    /* ---------------------------------------------------------------- */
    /* Compute terms up to the requested index                        */
    /* ---------------------------------------------------------------- */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Next Lucas number: L_i = L_{i-1} + L_{i-2}                */
        seq_t luc_next = lucas_prev + lucas_prev2;
        lucas_prev2 = lucas_prev;
        lucas_prev  = luc_next;

        /* a(i) = a(i-1) * 10^{digit_len(L_i)} + L_i               */
        seq_t p10   = pow10(digit_len(luc_next));
        seq_t a_new = cache[computed - 1] * p10 + luc_next;

        /* Resize cache if necessary */
        if (computed >= cache_cap) {
            size_t new_cap = cache_cap << 1;   /* double the capacity */
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)
                abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[computed] = a_new;
        ++computed;
    }

    return cache[n - 1];
}
