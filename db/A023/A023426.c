/*
 * A023426  a(n) = a(n-1) + Sum_{k=0..n-4} a(k)*a(n-4-k), a(0) = 1.
 *          Generalized Catalan Numbers.
 * https://oeis.org/A023426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + Sum_{k=0}^{n-4} a(k)*a(n-4-k), a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;   /* Memoised values of a(n). */
static size_t cap     = 0;      /* Allocated size of cache.   */
static idx_t  next    = 0;      /* First index that has not yet been computed. */

/* Ensure that cache can hold values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    /* If we already have enough space, return. */
    if (cap > (size_t)n) return;

    size_t new_cap = ((size_t)n + 1) * 2;   /* Grow by a factor of 2. */
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);                  /* Abort on OOM. */
    cache = new_arr;
    cap   = new_cap;
    /* Newly allocated elements are uninitialised; they will be set
       during the computation loop. */
}

seq_t a(idx_t n)
{
    if (n < next)                     /* Already computed. */
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = next; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 1;             /* Base cases a(0)=a(1)=a(2)=a(3)=1. */
        } else {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i - 4; ++k)
                sum += cache[k] * cache[i - 4 - k];
            cache[i] = cache[i - 1] + sum;
        }
    }
    next = n + 1;                      /* All values up to n are now ready. */
    return cache[n];
}
