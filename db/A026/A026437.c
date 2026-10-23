/*
 * A026437  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for 1
 *          <= i <= j < n, n >= 2, with initial terms 1,2.
 * https://oeis.org/A026437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer > a(n‑1) that is not of the form
 *            a(i)*a(j)-1 for 1 ≤ i ≤ j < n, with a(1)=1, a(2)=2
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Global cache for the sequence.  It is grown dynamically. */
static seq_t *cache   = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

/* ---------- helper: binary search in the sorted cache ----------
   Returns true if `val` is present among the first `len` elements.  */
static bool contains(const seq_t *seq, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] < val)
            lo = mid + 1;
        else if (seq[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* ---------- helper: ensure the cache can hold at least `needed` items ----------
   The capacity is doubled until it is enough.  */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();   /* allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* ------------------ main function ---------------------------------
   Computes a(n) for any n >= 1, remembering all previously computed terms
   so that subsequent calls are fast.  */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        ensure_capacity(16);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    if (n <= 0)   /* undefined for n < 1, but avoid UB */
        return (seq_t)0;

    /* If we haven't computed up to n, generate the missing values. */
    if (cache_len < (size_t)n) {
        for (size_t cur = cache_len; cur < (size_t)n; ++cur) {
            seq_t cand = cache[cache_len - 1] + (seq_t)1; /* start from previous+1 */
            while (1) {
                seq_t val = cand + (seq_t)1;   /* we need val = cand+1 to check a(i)*a(j) */
                bool bad = false;

                /* Check whether val can be expressed as product of two earlier terms. */
                for (size_t i = 0; i < cache_len; ++i) {
                    seq_t ai = cache[i];
                    seq_t div = val / ai;
                    /* If ai > sqrt(val) we are done. */
                    if (div < ai)
                        break;
                    if (val % ai == (seq_t)0) {
                        seq_t t = div;          /* t = val / ai */
                        if (contains(cache, cache_len, t)) {
                            bad = true;
                            break;
                        }
                    }
                }

                if (!bad)          /* candidate accepted */
                    break;

                ++cand;            /* try next integer */
            }

            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cand;   /* store the new term */
        }
    }

    return cache[n - 1];
}
