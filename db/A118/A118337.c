/*
 * A118337  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 23)^2 = y^2.
 * https://oeis.org/A118337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 72, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118337: non‑negative x of solutions to x² + (x+23)² = y²
   Recurrence:  a(n) = 6*a(n-3) - a(n-6) + 46  for n > 6
   Initial values (1‑based):
   a(1)=0, a(2)=12, a(3)=33, a(4)=69, a(5)=133, a(6)=252
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; indices are 1‑based */
    static seq_t *cache = NULL;   /* array of size cap, indices 0..cap-1 */
    static size_t cap   = 0;      /* allocated array size */
    static idx_t  size  = 0;      /* highest index that has been computed (0 means none) */

    /* First call – allocate space for the first six values. */
    if (cache == NULL) {
        cap = 8;                    /* enough room for indices 0..7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* OOM guard – return 0 */

        /* initialise indices 1..6 (un‑used 0th slot set to 0) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)69;
        cache[5] = (seq_t)133;
        cache[6] = (seq_t)252;
        size = 6;
    }

    /* Return immediately if the requested term is already known. */
    if (n <= size) return cache[n];

    /* Grow the array if we need to store indices beyond the current capacity. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* OOM guard – return 0 */
        cache = tmp;
        cap  = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = size + 1; i <= n; ++i) {
        /* a(i) = 6*a(i‑3) – a(i‑6) + 46 */
        cache[i] = cache[i-3] * (seq_t)6 - cache[i-6] + (seq_t)46;
    }
    size = n;
    return cache[n];
}
