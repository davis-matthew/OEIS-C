/*
 * A169092  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from the rational *
 *            generating function G(t)=N(t)/D(t): * * a(0)=1, * for all n ≥ 0
 *            * a(n) - 29a(n-1) - ⋯ - 29a(n-24) + 435a(n-25) = c(n), * * where
 *            the inhomogeneous term * * c(n) = 1 if n = 0 or n = 25, * 2 if 1
 *            ≤ n ≤ 24, * 0 otherwise. * * The recur...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* array of previously computed a(k) */
static idx_t size  = 0;       /* number of values stored (next index to compute) */
static idx_t cap   = 0;       /* allocated capacity of cache */

/* Allocate or enlarge the cache to hold at least 'new_cap' elements.  */
static void ensure_cap(idx_t new_cap)
{
    if (new_cap <= cap)
        return;
    seq_t *new_cache = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* memory allocation failed */
    for (idx_t i = 0; i < size; ++i)
        new_cache[i] = cache[i];
    if (cache)
        free(cache);
    cache = new_cache;
    cap   = new_cap;
}

/* Return a(n).  Uses the recurrence described above and memoises results. */
seq_t a(idx_t n)
{
    if (n >= size) {                     /* need to compute more terms */
        ensure_cap(n + 1);                /* make sure we have space */
        for (idx_t k = size; k <= n; ++k)
        {
            seq_t val = 0;

            /* 29 * sum of a(k-i) for i = 1..24  (only terms that exist) */
            for (idx_t i = 1; i <= 24; ++i)
                if (k >= i)
                    val += 29 * cache[k - i];

            /* subtract 435 * a(k-25)  if it exists */
            if (k >= 25)
                val -= 435 * cache[k - 25];

            /* add the inhomogeneous term c(k) */
            if (k == 0 || k == 25)
                val += 1;
            else if (k <= 24)          /* 1 <= k <= 24 */
                val += 2;

            cache[k] = val;
        }
        size = n + 1;                       /* all values up to n are now stored */
    }
    return cache[n];
}
