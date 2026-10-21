/*
 * A209013  Number of n X 2 0..3 arrays with no element equal the average of
 *          immediate neighbors vertically above, diagonally above and left,
 *          and horizontally left of it.
 * https://oeis.org/A209013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209013
   Empirical recurrence:
   a(n) = 8*a(n-1) + 38*a(n-2) - 52*a(n-3) - 40*a(n-4) + 44*a(n-5) - 12*a(n-6)
   base values:
   a(1)=12, a(2)=130, a(3)=1430, a(4)=15724, a(5)=172936, a(6)=1901964
*/

#include <stdlib.h>

static seq_t init_vals[6] = {
    (seq_t)12,      /* a(1) */
    (seq_t)130,     /* a(2) */
    (seq_t)1430,    /* a(3) */
    (seq_t)15724,   /* a(4) */
    (seq_t)172936,  /* a(5) */
    (seq_t)1901964  /* a(6) */
};

static seq_t *cache = NULL;    /* cache[1..cur_max] holds computed values */
static idx_t cur_max = 0;      /* maximum index currently stored */
static size_t alloc_size = 0;  /* allocated array size (index 0 unused) */

/* Allocate or grow the cache to hold indices up to `need`.  Zero‑terminates if needed. */
static void ensure_cache(idx_t need)
{
    if (alloc_size >= need + 1)
        return;                      /* already large enough */

    seq_t *newc = (seq_t*)malloc((size_t)(need + 1) * sizeof(seq_t));
    if (newc == NULL)
        return;                      /* out of memory – caller may handle */

    /* copy old contents */
    if (cache != NULL) {
        for (idx_t i = 1; i <= cur_max; ++i)
            newc[i] = cache[i];
        free(cache);
    }

    cache = newc;
    alloc_size = (size_t)(need + 1);

    /* initialise base values if not already present */
    if (cur_max < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i - 1];
        cur_max = 6;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* below offset – undefined, return 0 */
        return (seq_t)0;

    if (n <= (idx_t)6)           /* base values */
        return init_vals[n - 1];

    ensure_cache(n);             /* make sure cache can hold up to n */

    if (n > cur_max) {           /* compute missing terms */
        for (idx_t i = cur_max + 1; i <= n; ++i) {
            cache[i] =
                (seq_t)8  * cache[i - 1] +
                (seq_t)38 * cache[i - 2] -
                (seq_t)52 * cache[i - 3] -
                (seq_t)40 * cache[i - 4] +
                (seq_t)44 * cache[i - 5] -
                (seq_t)12 * cache[i - 6];
        }
        cur_max = n;
    }

    return cache[n];
}
