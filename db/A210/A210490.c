/*
 * A210490  Union of positive squares (A000290 \ {0}) and squarefree numbers
 *          (A005117).
 * https://oeis.org/A210490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210490: a(n) is the n‑th positive integer that is either a perfect square
   (counting 1) or a squarefree number; i.e., all exponents in the prime
   factorisation are even or all are 1.  The function generates terms on
   demand using a simple incremental search. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/floor_sqrt.h"

/* static cache to allow out‑of‑order queries */
static seq_t *cache   = NULL;      /* array of stored terms          */
static size_t cache_len = 0;       /* how many terms we have stored */
static size_t cache_cap = 0;       /* allocated capacity of cache    */
static seq_t next_num = 1;         /* next integer to test           */

/* enlarge the cache to at least `needed` entries */
static void ensure_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;

#ifdef __cplusplus
    /* reallocate memory in C++   */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#else
    /* reallocate memory in C     */
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp)            /* allocation failed – ignore (should not happen) */
        return;

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but guard against misuse */

    /* Generate terms until we have at least `n` entries */
    while (cache_len < (size_t)n) {

        bool ok = is_squarefree(next_num);
        if (!ok) {
            seq_t r = floor_sqrt(next_num);
            if (r * r == next_num)
                ok = true;          /* perfect square */
        }

        if (ok) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = next_num;
        }

        ++next_num;
    }

    return cache[n - 1];
}
