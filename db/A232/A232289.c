/*
 * A232289  Number of nX2 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally.
 * https://oeis.org/A232289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A232289
   Recurrence: a(n) = 4*a(n-1) + 4*a(n-2) + a(n-3)
   Initial values: a(1)=3, a(2)=15, a(3)=73 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; index in array is a(n) at position n-1   */
    static idx_t max_n = 0;          /* highest n computed so far      */
    static size_t cap   = 0;          /* capacity (in elements)         */
    static seq_t *cache = NULL;      /* dynamic cache                  */

    /* allocate and initialise cache on first call (at least 3 elements) */
    if (cap < 3) {
        size_t new_cap = 3;
        seq_t *ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) {
            /* Allocation failure; in the context of sequence generation
               this is a fatal error – aborting is acceptable since the
               caller is expected to handle out‑of‑memory gracefully. */
            abort();
        }
        cache = ptr;
        cap   = new_cap;
    }

    /* Ensure that the first three terms are present */
    if (max_n < 3) {
        cache[0] = (seq_t)3;   /* a(1) */
        cache[1] = (seq_t)15;  /* a(2) */
        cache[2] = (seq_t)73;  /* a(3) */
        if (max_n == 0) max_n = 3;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_n) {
        return cache[(size_t)(n - 1)];
    }

    /* Extend the cache if needed */
    if (cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();
        cache = ptr;
        cap   = new_cap;
    }

    /* Compute successive terms until n is reached */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* i is 1‑based.  For i ≥ 4 we can use the recurrence.  The loop
           starts at max_n+1, which is at least 4 after the initialisation
           of the first three values.                                      */
        seq_t val = 4 * cache[(size_t)(i - 2)]     /* 4*a(i-1) */
                 + 4 * cache[(size_t)(i - 3)]     /* 4*a(i-2) */
                 +   cache[(size_t)(i - 4)];      /*   a(i-3) */
        cache[(size_t)(i - 1)] = val;
    }
    max_n = n;
    return cache[(size_t)(n - 1)];
}
