/*
 * A013549  Incorrect version of A000085.
 * https://oeis.org/A013549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the number of involutions (telephone numbers):
   a(0)=1, a(1)=1, a(n)=a(n-1)+(n-1)*a(n-2).
   The OEIS entry A013549 contains an erroneous entry at n=5;
   the value 41 is returned for n==5 to match the given b‑file. */

#include <stdlib.h>

static seq_t *inv_cache = NULL;   /* cache for computed values */
static idx_t   inv_len   = 0;     /* highest index computed in cache */
static size_t  inv_cap   = 0;     /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (inv_cache == NULL) {
        size_t init = (size_t)(n + 1);
        if (init < 4) init = 4;               /* at least enough for indices 0..3 */
        inv_cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (inv_cache == NULL) return (seq_t)0;
        inv_cache[0] = (seq_t)1;
        inv_cache[1] = (seq_t)1;
        inv_len = 1;
        inv_cap = init;
    }

    /* Expand storage if necessary */
    if ((size_t)(n + 1) > inv_cap) {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(inv_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        inv_cache = tmp;
        inv_cap   = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    if (inv_len < n) {
        for (idx_t i = inv_len + 1; i <= n; ++i) {
            inv_cache[i] = inv_cache[i-1] + ((seq_t)(i-1)) * inv_cache[i-2];
        }
        inv_len = n;
    }

    /* Correct the erroneous term at n = 5 */
    if (n == 5) return (seq_t)41;

    return inv_cache[n];
}
