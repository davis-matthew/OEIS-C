/*
 * A024735  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=2 and a(2)=a(3)=1.
 * https://oeis.org/A024735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..⌊(n+1)/4⌋} a(2*i-1) * a(n-2*i+1) (OEIS A024735)
 *            Initial values: a(1)=2, a(2)=a(3)=1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below works for any integer seq_t (fixed‑size or arbitrary‑precision) and
   any unsigned 64‑bit idx_t. It keeps a static cache and builds the
   sequence on demand in a bottom‑up manner, so calls may arrive in any
   order. */

#include <stdlib.h>
#include <stddef.h>

/* static cache; cache[0] unused (0 for all types) */
static seq_t *cache = NULL;
/* current cache capacity (≥ last computed index + 1) */
static size_t cap = 0;
/* highest index computed so far */
static size_t last = 0;

/* Ensure the cache can hold indices up to n (inclusive).  New
   entries are initialized to zero. */
static void
ensure_cap(idx_t n)
{
    if ((idx_t)cap > n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* out of memory */
    /* initialize new entries to zero */
    for (size_t i = cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cap = new_cap;
}

/* Compute a(n) using a bottom‑up cache.  The sequence only depends on
   values of smaller indices, so we can fill the cache iteratively
   up to n. */
static seq_t
compute_a(idx_t n)
{
    if (n == 0)
        return 0;                 /* not used */

    ensure_cap(n);

    /* Fill cache entries that are still unknown, starting after last. */
    for (size_t k = last + 1; k <= (size_t)n; ++k)
    {
        if (k == 1)
        {
            cache[k] = 2;
        }
        else if (k == 2 || k == 3)
        {
            cache[k] = 1;
        }
        else
        {
            seq_t sum = 0;
            idx_t max = (idx_t)(k + 1) / 4;
            for (idx_t i = 1; i <= max; ++i)
            {
                /* indices used are < k, so already computed */
                seq_t left  = cache[2 * i - 1];
                seq_t right = cache[k - 2 * i + 1];
                sum += left * right;
            }
            cache[k] = sum;
        }
    }
    last = (size_t)n;
    return cache[n];
}

/* Public function required by the test harness.  It is safe for any
   order of calls and for any build target. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
