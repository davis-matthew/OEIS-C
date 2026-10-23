/*
 * A026417  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<j<=n, with initial terms 1,3.
 * https://oeis.org/A026417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) that is not expressible
 *            as a product a(i)*a(j) for 1 ≤ i < j ≤ n-1; a(1)=1, a(2)=3
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the OEIS sequence A026417.  The sequence is stored in a
   static array so that the function may be called in arbitrary order.
   Each term is found by searching upwards from the previous term
   until it is not a product of two earlier terms. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array of computed terms */
    static idx_t len = 0;       /* number of computed terms */
    static idx_t cap = 0;       /* current allocation capacity */

    /* Return what we already know if possible. */
    if (n <= len)
        return seq[n-1];

    /* Ensure we have storage for at least n terms. */
    if (cap == 0) {
        cap = 8;
        seq = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* out of memory */
    }
    if (cap < n) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;
        cap = new_cap;
    }

    /* Initialise the first two terms if not already done. */
    if (len == 0) { seq[0] = (seq_t)1; len = 1; }
    if (len == 1) { seq[1] = (seq_t)3; len = 2; }

    /* Generate terms up to the requested index. */
    for (idx_t k = len; k < n; ++k) {
        seq_t cand = seq[k-1] + (seq_t)1;   /* start at previous term + 1 */
        for (;;) {
            bool is_product = false;
            for (idx_t i = 0; i < k; ++i) {
                seq_t ai = seq[i];
                if ((cand % ai) == 0) {
                    seq_t y = cand / ai;
                    /* binary search for y among earlier terms */
                    idx_t lo = 0, hi = k;
                    while (lo < hi) {
                        idx_t mid = (lo + hi) >> 1;
                        if (seq[mid] < y) lo = mid + 1;
                        else hi = mid;
                    }
                    if (lo < k && seq[lo] == y && lo > i) {
                        is_product = true;
                        break;
                    }
                }
            }
            if (!is_product) {
                seq[k] = cand;
                break;
            }
            cand = cand + (seq_t)1;   /* try next candidate */
        }
        len = k + 1;
    }

    return seq[n-1];
}
