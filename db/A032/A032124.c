/*
 * A032124  "BIK" (reversible, indistinct, unlabeled) transform of 2,2,2,2...
 * https://oeis.org/A032124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=5, a(3)=12;
   for n≥4: a(n)=3*a(n‑1)+3*a(n‑2)-9*a(n‑3) */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *a_cache = NULL;   /* memoization array */
static size_t   a_cache_cap = 0;/* allocated size (elements)         */
static idx_t    a_cache_max = 0;/* highest index computed so far      */

/* Make the cache large enough to hold indices 0..n. */
static void
grow_a_to(idx_t n)
{
    if (a_cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache      = tmp;
        a_cache_cap  = (size_t)(n + 1);
    }
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        if (i == 1)                a_cache[i] =  (seq_t)2;
        else if (i == 2)           a_cache[i] =  (seq_t)5;
        else if (i == 3)           a_cache[i] =  (seq_t)12;
        else {
            seq_t v = a_cache[i-1] * 3
                    + a_cache[i-2] * 3
                    - a_cache[i-3] * 9;
            a_cache[i] = v;
        }
    }
    a_cache_max = n;
}

/* Make sure the value for index n is computed. */
static void
ensure_a(idx_t n)
{
    if (a_cache_max < n) grow_a_to(n);
}

/* Public interface.  a(1) is the first term; arguments <1 are ignored. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behavior below the offset is irrelevant */
    ensure_a(n);
    return a_cache[n];
}
