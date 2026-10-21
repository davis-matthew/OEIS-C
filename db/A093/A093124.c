/*
 * A093124  a(n) = Sum_{i=n-k..n} sigma(i) which is not less than a(n-1) and
 *          obtained with the least k.
 * https://oeis.org/A093124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093124: a(n) = minimal suffix sum of σ(i) ending at n that
   is ≥ a(n-1); k is as small as possible.  The implementation keeps
   a cache of precomputed values so that calls in any order are
   answered in O(1) after the necessary range has been calculated. */

#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/sigma_of.h"

static size_t cache_cap = 0;            /* array capacity */
static seq_t *sigma_arr = NULL;         /* σ(i) */
static seq_t *pref_arr  = NULL;         /* prefix sums of σ */
static seq_t *a_arr     = NULL;         /* A093124 values */

/* Ensure the cache can accommodate at least n+1 elements. */
static void cache_ensure(idx_t n)
{
    if ((idx_t)cache_cap >= n + 1)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *tmp;
    tmp = (seq_t *)realloc(sigma_arr, new_cap * sizeof(seq_t));
    if (!tmp) return;  sigma_arr = tmp;
    tmp = (seq_t *)realloc(pref_arr,  new_cap * sizeof(seq_t));
    if (!tmp) return;  pref_arr  = tmp;
    tmp = (seq_t *)realloc(a_arr,     new_cap * sizeof(seq_t));
    if (!tmp) return;  a_arr     = tmp;

    cache_cap = new_cap;
}

/* Build the sequence up to n, filling sigma_arr, pref_arr, and a_arr. */
static void build_upto(idx_t n)
{
    cache_ensure(n);
    static idx_t built = 0;          /* last index that has been computed */

    for (idx_t i = built + 1; i <= n; ++i) {
        seq_t sig = sigma_of(i);
        sigma_arr[i] = sig;
        pref_arr[i]  = pref_arr[i - 1] + sig;

        if (i == 1) {
            a_arr[1] = sig;          /* σ(1) = 1 */
        } else {
            seq_t target = a_arr[i - 1];
            seq_t limit  = pref_arr[i] - target;   /* pref[t] ≤ limit */

            idx_t lo = 0, hi = i - 1, best = 0;
            while (lo <= hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                if (pref_arr[mid] <= limit) {
                    best = mid;
                    lo   = mid + 1;
                } else {
                    if (mid == 0) break;
                    hi   = mid - 1;
                }
            }
            a_arr[i] = pref_arr[i] - pref_arr[best];
        }
    }
    built = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* not defined for the sequence */

    build_upto(n);
    return a_arr[n];
}
