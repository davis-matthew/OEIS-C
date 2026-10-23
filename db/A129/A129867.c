/*
 * A129867  Row sums of triangular array T: T(j,k) = k*(j-k)! for k < j,
 *          T(j,k) = 1 for k = j; 1 <= k <= j.
 * https://oeis.org/A129867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n-1} k·(n‑k)!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the row sums of the triangular array T with
 *  T(j,k) = k·(j‑k)! for k < j, and T(j,j) = 1, where 1 ≤ k ≤ j.
 *  The function caches factorials to allow arbitrary ordering of calls.
 *  For n == 0 the return value is undefined; the function returns 0
 *  for convenience. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined case, return 0 */

    /* Cached factorials: fact[i] = i!  (0 ≤ i ≤ cached_last) */
    static seq_t *fact = NULL;
    static size_t capacity = 0;
    static size_t cached_last = 0;  /* last computed factorial index (>=0) */

    /* Ensure the cache holds factorials up to (n-1)! */
    idx_t need = n > 0 ? n - 1 : 0;   /* maximum factorial index needed */
    if (need >= cached_last) {
        /* Grow the allocation if necessary */
        size_t new_cap = capacity > 0 ? capacity : 64;
        while (new_cap <= (size_t)need) new_cap <<= 1;  /* double until enough */
        if (new_cap > capacity) {
            seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            fact = tmp;
            capacity = new_cap;
        }
        /* Compute missing factorials */
        for (idx_t i = cached_last; i <= need; ++i) {
            if (i == 0) {
                fact[0] = (seq_t)1;
            } else {
                fact[i] = fact[i-1] * (seq_t)i;
            }
        }
        cached_last = need;
    }

    /* Compute the row sum: 1 + Σ_{k=1}^{n-1} k·(n-k)! */
    seq_t sum = (seq_t)1;   /* the term T(n,n) = 1 */
    for (idx_t k = 1; k < n; ++k) {
        sum += (seq_t)k * fact[n - k];
    }

    return sum;
}
