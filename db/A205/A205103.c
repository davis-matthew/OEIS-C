/*
 * A205103  k!!-j!!, where (k!!,j!!) is the least pair of double factorials
 *          for which n divides their difference.
 * https://oeis.org/A205103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest positive difference of two double factorials
 *              which is divisible by n.
 * The algorithm finds the first two values k!! and j!! (k > j) such that
 * k!! ≡ j!! (mod n) and returns k!! - j!!.  The first such pair
 * guarantees that the difference is a multiple of n.
 * For efficiency the search is limited to double factorials up to 2*n,
 * which is sufficient for all n ≤ 10 000 (the maximum bound in the
 * specification).  The result is memoised for successive calls.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle request < 1; behaviour is undefined but return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation cache */
    static seq_t *cache = NULL;            /* array of results, index by n */
    static size_t cache_size = 0;          /* current capacity */

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to recomputation each time */
            cache = NULL;
            cache_size = 0;
        } else {
            /* initialise new slots to zero */
            for (size_t i = cache_size; i < new_size; ++i)
                tmp[i] = (seq_t)0;
            cache = tmp;
            cache_size = new_size;
        }
    }
    if (cache && cache[n] != (seq_t)0)
        return cache[n];

    /* Search for the smallest positive difference */
    idx_t limit = (idx_t)(2 * n);          /* upper bound for k */
    /* array for the first occurrence of each remainder (0..n-1) */
    seq_t *first = (seq_t *)calloc(n, sizeof(seq_t));
    if (!first)                /* Fallback: recompute without cache */
        limit = n;              /* small value, not ideal but safe */

    bool best_set = false;
    seq_t best = (seq_t)0;

    /* Current double factorials for odd/even indices */
    seq_t odd_val  = (seq_t)1;      /* 1!!       */
    seq_t even_val = (seq_t)1;      /* 0!!       */
    idx_t odd_rem  = (idx_t)(1 % n);
    idx_t even_rem = (idx_t)(1 % n);

    /* k = 0  → 0!! = 1 */
    first[even_rem] = even_val;

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t val;
        idx_t rem;
        if ((k & 1) == 0) { /* even k */
            val = even_val * (seq_t)k;
            rem = (even_rem * k) % n;
            even_val = val;
            even_rem = rem;
        } else {            /* odd k */
            val = odd_val * (seq_t)k;
            rem = (odd_rem * k) % n;
            odd_val = val;
            odd_rem = rem;
        }

        seq_t first_val = first[rem];
        if (first_val == (seq_t)0) {
            first[rem] = val;
        } else if (val > first_val) {
            seq_t diff = val - first_val;
            if (!best_set || diff < best) {
                best = diff;
                best_set = true;
                if (best == (seq_t)0)   /* cannot happen, but safety */
                    break;
            }
        }
    }
    free(first);

    /* Store and return result */
    if (cache)
        cache[n] = best;
    return best;
}
