/*
 * A026441  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for 1
 *          <= i <= j < n, n >= 2, with initial terms 2,4.
 * https://oeis.org/A026441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026441: a(1)=2, a(2)=4;
   a(n) = the least integer > a(n‑1) that is not of the form
   a(i)*a(j) - 1 for any 1 ≤ i ≤ j < n. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* binary search for 'value' in a[0 … sz-1] (sorted ascending) */
static bool seq_contains(const seq_t *a, size_t sz, seq_t value)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = a[mid];
        if (midv < value)
            lo = mid + 1;
        else if (midv > value)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* sequence generator */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq   = NULL;
    static size_t  size = 0;   /* number of stored terms   */
    static size_t  cap  = 0;   /* allocated capacity      */

    /* A026441 is defined for n ≥ 1                      */
    if (n == 0)
        return 0;                /* undefined for n==0      */

    /* initialise first two terms once */
    if (size == 0) {
        cap = 16;
        seq = seq_alloc(cap);
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)4;
        size  = 2;
    }

    /* extend the sequence until we have at least n terms */
    while (size < n) {
        seq_t prev = seq[size - 1];
        seq_t cand = prev + (seq_t)1;   /* start searching above a(n‑1) */

        for (;;) {
            seq_t target   = cand + (seq_t)1;   /* we look for target = a(i)*a(j) */
            bool forbidden = false;

            for (size_t i = 0; i < size; ++i) {
                seq_t ai = seq[i];
                if (ai > target - (seq_t)1)
                    break;                     /* further ai too large */

                if (target % ai == (seq_t)0) {
                    seq_t q = target / ai;
                    /* require j ≥ i ⇒ q ≥ ai, and q must already appear */
                    if (q >= ai && seq_contains(seq, size, q)) {
                        forbidden = true;
                        break;
                    }
                }
            }

            if (!forbidden)
                break;              /* found a valid candidate */

            ++cand;                 /* try next integer */
        }

        /* store the new term */
        if (size >= cap) {
            cap = cap * 2;
            seq = seq_realloc(seq, cap/2, cap);  /* old_cap was cap/2 */
        }
        seq[size++] = cand;
    }

    /* offset 1 → a(n) is at index n-1 */
    return seq[n - (idx_t)1];
}
