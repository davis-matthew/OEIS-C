/*
 * A233093  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, diagonally or antidiagonally, top
 *          left element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233093
 *  Number of n×3 0..3 arrays with no adjacent (horizontally,
 *  diagonally or antidiagonally) values x and 3‑x, top‑left 0 and
 *  1 appearing before 2 in row major order.
 *
 *  Recurrence (n ≥ 4):
 *      a(1) = 5
 *      a(2) = 65
 *      a(3) = 941
 *      a(n) = 16*a(n-1) - 21*a(n-2) + 6*a(n-3)
 *
 *  The function memoises results so calls may arrive in any order.
 */

#include <stdlib.h>

/* Cached values (1‑based).  cache[0] is unused. */
static seq_t *cache = NULL;
/* Number of allocated slots (>= 1 if cache != NULL). */
static size_t cap = 0;
/* How far the cache has been computed (max index stored). */
static idx_t computed = 0;

/* Grow the cache so that indices up to need are valid. */
static void
grow_cache(idx_t need)
{
    if ((size_t)need + 1 <= cap)
        return;
    size_t new_cap;
    if (cap == 0)
        new_cap = need + 1;
    else
        new_cap = cap;
    while ((size_t)need + 1 > new_cap)
        new_cap <<= 1;   /* double until large enough */

#ifdef __cplusplus
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (tmp == NULL)
        abort();         /* allocation failure */
    cache = tmp;
    cap = new_cap;
}

/* Initialise the base values if they have not been set yet. */
static void
init_cache(void)
{
    if (computed == 0) {
        grow_cache(3);              /* need indices 1..3 */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)65;
        cache[3] = (seq_t)941;
        computed = 3;
    }
}

/* Public interface.  Computes a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* offset is 1; undefined below 1 */

    init_cache();                  /* ensure base values exist */

    if (n <= computed)
        return cache[n];           /* already computed */

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cap)
        grow_cache(n);

    /* Compute missing terms. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1];
        seq_t term2 = cache[i - 2];
        seq_t term3 = cache[i - 3];
        cache[i] = (seq_t)16 * term1 - (seq_t)21 * term2 + (seq_t)6 * term3;
    }
    computed = n;
    return cache[n];
}
