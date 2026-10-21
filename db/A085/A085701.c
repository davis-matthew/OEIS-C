/*
 * A085701  Smallest multiple of n such that a(n) + a(n-1) is a squarefree
 *          number.
 * https://oeis.org/A085701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = smallest multiple of n, m = k*n, k≥1, such that
 *            a(n)+a(n-1) is square‑free. Initial value: a(1) = 1. The
 *            sequence is defined only up to n = 319 (after which no such
 *            multiple exists). This implementation memoises computed values
 *            and stops searching if no square‑free sum can be found within...
 * Range:     u8 n <= 77, u16 n <= 319, u32 n <= 319, u64 n <= 319,
 *            u128 n <= 319, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Helper: compute a(n) with memoisation.
   The function is thread‑safe only when calls are serial; if called concurrently
   more robust synchronisation is required, but this is not part of the specification. */
static seq_t *cache = NULL;             /* 1‑based index cache */
static idx_t cache_size = 0;             /* allocated size (terms +1 for zero index) */
static idx_t computed   = 0;             /* highest index computed so far */

/* Final index for which the sequence is defined.  For n > 319 no value exists,
   so the function simply returns 0. */
#define MAX_DEFINED_N 319

seq_t a(idx_t n)
{
    /* 1‑based indexing: ignore n == 0, but return 0 for safety. */
    if (n == 0) return 0;

    /* For n larger than the defined range, no term exists. */
    if (n > MAX_DEFINED_N) return 0;

    /* Expand cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1; /* include zero index */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* If already computed, return the stored value */
    if (n <= computed) return cache[n];

    /* Initialise first term if needed */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t k = 1;
        while (1) {
            /* candidate multiple of i */
            seq_t cand = (seq_t)k * (seq_t)i;
            /* sum with previous term */
            seq_t sum   = cand + prev;
            if (is_squarefree(sum)) {
                cache[i] = cand;
                break;
            }
            ++k;
            /* If k becomes too large, give up and set 0.
               In practice the sequence is defined up to 319 and k stays small. */
            if (k > 1000000) {
                cache[i] = 0;
                break;
            }
        }
    }
    computed = n;
    return cache[n];
}
