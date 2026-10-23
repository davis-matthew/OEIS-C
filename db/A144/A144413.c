/*
 * A144413  a(n) = Sum_{k=0..n} (-1)^k * binomial(n, k) * A000931(n-k+4).
 * https://oeis.org/A144413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 39, i64 n <= 81, i128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144413:
 * a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k) * A000931(n-k+4)
 * This sequence satisfies the recurrence
 *   a(0) = 0, a(1) = 1, a(2) = -1,
 *   a(n) = -3*a(n-1) - 2*a(n-2) + a(n-3)   for n ≥ 3.
 */

#include <stdlib.h>

static seq_t *A144413_cache = NULL;
static idx_t  A144413_cache_size = 0;

/* Keep a memoised table; grow it only when needed. */
static void
A144413_ensure_up_to(idx_t n)
{
    if (n + 1 <= A144413_cache_size)            /* already present */
        return;

    idx_t old_size = A144413_cache_size;
    idx_t new_size = n + 1;
    if (new_size < 3)                           /* always store at least first 3 terms */
        new_size = 3;

    seq_t *tmp = (seq_t *)realloc(A144413_cache, new_size * sizeof(seq_t));
    A144413_cache = tmp;

    /* initialise the first three values (overwrites if they existed) */
    if (new_size > 0) A144413_cache[0] = 0;
    if (new_size > 1) A144413_cache[1] = 1;
    if (new_size > 2) A144413_cache[2] = -1;

    /* compute new entries beyond what we already had */
    idx_t start = old_size;
    if (start < 3)
        start = 3;             /* first non‑seed term */

    for (idx_t i = start; i <= n; ++i) {
        A144413_cache[i] =
            -3 * A144413_cache[i-1]
            - 2 * A144413_cache[i-2]
            +   A144413_cache[i-3];
    }

    A144413_cache_size = new_size;
}

seq_t a(idx_t n)
{
    A144413_ensure_up_to(n);
    return A144413_cache[n];
}
