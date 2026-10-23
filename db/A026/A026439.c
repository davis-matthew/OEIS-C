/*
 * A026439  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for 1
 *          <= i <= j < n, n >= 2, with initial terms 2,3.
 * https://oeis.org/A026439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026439: a(1)=2, a(2)=3.
   For n≥3, a(n) is the smallest integer > a(n‑1) such that
   a(n)+1 is not a product a(i)*a(j) with 1 ≤ i ≤ j < n. */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* binary search for key in sorted array a[low..high] (inclusive) */
static idx_t
binary_search(const seq_t *a, idx_t low, idx_t high, seq_t key)
{
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t m = a[mid];
        if (m == key) return mid;
        if (m < key) low = mid + 1;
        else high = mid - 1;
    }
    return (idx_t)-1;
}

/* ensure allocated capacity is at least needed */
static void
ensure_capacity(idx_t needed, idx_t *cap, seq_t **seq)
{
    if (needed <= *cap) return;
    idx_t new_cap = *cap ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    if (!*seq) {
        *seq = seq_alloc(new_cap);
    } else {
        *seq = seq_realloc(*seq, *cap, new_cap);
    }
    *cap = new_cap;
}

/* external sequence function */
seq_t a(idx_t n)
{
    static seq_t *seq = (seq_t *)0;
    static idx_t cap = 0;
    static idx_t len = 0;

    if (n == 0) return 0;        /* offset starts at 1 */

    /* initialise first two terms */
    if (len == 0) {
        ensure_capacity(2, &cap, &seq);
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)3;
        len    = 2;
    }

    /* compute up to requested n */
    while (len < n) {
        seq_t candidate = seq[len - 1] + 1;   /* start just above previous term */
        for (;;) {
            seq_t target = candidate + 1;     /* must not equal a[i]*a[j] */
            int forbidden = 0;
            for (idx_t i = 0; i < len; ++i) {
                seq_t ai = seq[i];
                if (ai > target / ai) break;  /* further i cannot factor target */
                if (target % ai != 0) continue;
                seq_t rem = target / ai;
                idx_t pos = binary_search(seq, i, len - 1, rem);
                if (pos != (idx_t)-1) {
                    forbidden = 1;
                    break;
                }
            }
            if (!forbidden) break;           /* found a valid candidate */
            ++candidate;                     /* try next integer */
        }
        /* store the chosen term */
        ensure_capacity(len + 1, &cap, &seq);
        seq[len] = candidate;
        ++len;
    }

    return seq[n - 1];
}
