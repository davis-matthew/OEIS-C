/*
 * A079358  a(n) is taken to be the smallest positive integer greater than
 *          a(n-1) which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is not a multiple of either 3 or
 *          4.".
 * https://oeis.org/A079358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A079358
 *
 * Definition:
 *   a(1) = 1.
 *   For n > 1, a(n) is the smallest integer greater than a(n-1) that satisfies
 *   the following consistency condition:
 *     let M be the set of indices k such that a(k) == n has already appeared
 *     (i.e., n is a member of the sequence).
 *     - If n ∈ M, then a(n) must NOT be a multiple of 3 or 4.
 *     - If n ∉ M, then:
 *         * if a(n) == n, n becomes a member and a(n) must NOT be a multiple
 *         * otherwise n stays non‑member and a(n) must be a multiple of 3 or 4.
 *   This is equivalent to constructing the unique increasing sequence that
 *   obeys “n is a member of the sequence iff a(n) is not a multiple of 3 or 4”.
 *
 * The function builds the sequence incrementally, caching previously computed
 * terms and reusing them for later calls.  It uses only arithmetic and
 * comparison operators allowed by the specification and works for built‑in
 * unsigned types as well as arbitrary‑precision integer classes.
 */

#include <stdlib.h>
#include <stdbool.h>

/* caching of already computed terms: cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache = NULL;            /* array of a(k) for k = 1 .. cache_size */
static idx_t  cache_size = 0;           /* number of terms currently stored   */
static idx_t  cache_capacity = 0;       /* allocated capacity                 */

/* Ensure that the cache can hold at least 'min_cap' terms.  */
static void ensure_cache_capacity(idx_t min_cap)
{
    if (cache_capacity >= min_cap) return;

    idx_t new_cap = cache_capacity ? cache_capacity : 16;
    while (new_cap < min_cap) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Binary search: does 'target' exist in the first 'size' elements of 'arr'? */
static bool contains(const seq_t *arr, idx_t size, idx_t target)
{
    seq_t v = (seq_t)target;
    idx_t lo = 0, hi = size;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] < v) {
            lo = mid + 1;
        } else if (v < arr[mid]) {
            hi = mid;
        } else {
            return true;
        }
    }
    return false;
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* convention, should not be used */

    /* Build the cache up to index n. */
    while (cache_size < n) {
        /* index of the new term (1‑based). */
        idx_t idx = cache_size + 1;

        /* previous term a(idx-1); 0 if none. */
        seq_t prev = (cache_size == 0) ? 0 : cache[cache_size - 1];

        /* candidate larger than prev. */
        seq_t cand = prev + 1;

        /* Did n (the index) already appear as a term before? */
        bool prior_member = contains(cache, cache_size, idx);

        /* Find the smallest admissible candidate. */
        while (1) {
            bool mult = (cand % 3 == 0) || (cand % 4 == 0);
            bool ok;

            if (prior_member) {
                /* n is already a member → a(n) must NOT be a multiple. */
                ok = !mult;
            } else {
                /* n not yet a member.  Choosing cand == n would make it a member. */
                if ((seq_t)idx == cand) {
                    /* cand == n → must NOT be a multiple. */
                    ok = !mult;
                } else {
                    /* cand != n → keep n non‑member → must be a multiple. */
                    ok = mult;
                }
            }

            if (ok) break;
            ++cand;
        }

        /* Store the chosen value. */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size] = cand;
        ++cache_size;
    }

    return cache[n - 1];
}
