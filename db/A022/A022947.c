/*
 * A022947  a(n) = a(n-1) + c(n) for n >= 3, a( ) increasing, given a(1)=1
 *          a(2)=3; where c( ) is complement of a( ).
 * https://oeis.org/A022947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A022947
 *
 *  a(1) = 1,  a(2) = 3
 *  For n ≥ 3 :  a(n) = a(n‑1) + c(n)
 *  where c(n) is the n‑th positive integer that does NOT appear in
 *  the sequence {a(1), a(2), … , a(n‑1)}.  (c(n) is the nth member
 *  of the complement of the set of preceding a‑values.)
 *
 *  The function below keeps a memoised cache of all a‑values that
 *  have been computed so far.  c(n) is obtained by a binary search
 *  over the missing numbers using the current cache.
 */

#include <stdlib.h>

/* static cache of computed a‑values */
static seq_t *cache = NULL;   /* 0‑indexed: cache[0] == a(1)  */
static size_t cap = 0;        /* current allocation size      */
static size_t len = 0;        /* number of values present     */

/* Count how many a‑values are ≤ H (binary search). */
static inline size_t count_leq(idx_t H) {
    size_t low = 0, high = len;
    while (low < high) {
        size_t mid = (low + high) / 2;
        if (cache[mid] <= (seq_t)H)
            low = mid + 1;
        else
            high = mid;
    }
    return low;   /* number of a‑values ≤ H */
}

/* Missing numbers up to H:  (# of integers ≤H) – (# of a‑values ≤H) */
static inline seq_t missing_count(idx_t H) {
    seq_t X = (seq_t)H;
    size_t cnt = count_leq(H);
    return X - (seq_t)cnt;
}

/* Return the k‑th missing positive integer.  Uses the current cache
   of a‑values (up to index k‑2).  */
static inline seq_t kth_missing(idx_t k) {
    idx_t low  = 1;
    idx_t high = k;               /* initial guess */

    /* enlarge upper bound until we have at least k missing numbers */
    while (missing_count(high) < k) {
        idx_t new_high = high + k;
        if (new_high <= high)          /* overflow protection */
            new_high = (idx_t)(-1);
        high = new_high;
    }

    /* binary search: smallest x with missing_count(x) ≥ k */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (missing_count(mid) >= k)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

/* Main sequence function:  a(n)  */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* initialise cache on first use */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        len = 2;
    }

    /* return cached value if already known */
    if (n <= len) return cache[n-1];

    /* compute new terms up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        seq_t c_val = kth_missing(k);
        seq_t new_val = cache[len-1] + c_val;

        /* grow cache if necessary */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap = new_cap;
        }
        cache[len] = new_val;
        ++len;
    }

    return cache[n-1];
}
