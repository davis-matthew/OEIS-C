/*
 * A118029  a(0)=1. a(n) = a(n-1) + (smallest integer that is >= n and occurs
 *          among the earlier terms of the sequence).
 * https://oeis.org/A118029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 352, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1,
   a(n) = a(n‑1) + min{a(k) | k < n and a(k) ≥ n} */
/* This implementation memoises the sequence and uses a binary
   search for the required minimum among the already computed terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Binary search for the first element in a[0..len-1] that is
   greater than or equal to val.  The array is assumed sorted. */
static inline idx_t
lower_bound(const seq_t *a, idx_t len, seq_t val)
{
    idx_t lo = 0;
    idx_t hi = len;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (a[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Memoised sequence generator.  It stores all computed terms in a
   dynamically resized array.  Subsequent calls in any order are
   answered in O(log n) time. */
seq_t
a(idx_t n)
{
    static seq_t *buf = NULL;      /* array of computed values   */
    static size_t cap = 0;         /* current allocation capacity */
    static idx_t len = 0;          /* number of stored terms      */

    /* Initialise the array with the first term a(0)=1. */
    if (!buf) {
        cap = 16;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return 0;        /* allocation failure – return 0 */
        buf[0] = (seq_t)1;
        len = 1;
    }

    /* Compute terms up to and including a(n). */
    while (len <= n) {
        /* Grow the buffer if necessary. */
        if ((size_t)len == cap) {
            size_t new_cap = cap << 1;
            if (new_cap == 0) new_cap = 1;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) return 0;    /* allocation failure – return 0 */
            buf = tmp;
            cap = new_cap;
        }

        /* Index of the term being computed. */
        idx_t idx = len;

        /* Find the smallest previously computed term that is ≥ idx. */
        idx_t pos = lower_bound(buf, idx, (seq_t)idx);
        seq_t t = buf[pos];

        /* Compute a(idx) = a(idx‑1) + t. */
        buf[idx] = buf[idx - 1] + t;

        ++len;
    }

    return buf[n];
}
