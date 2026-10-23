/*
 * A026070  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0, |s(i) - s(i-1)| = 1 for i = 1,2; |s(i) -
 *          s(i-1)| <= 1 for i >= 3, s(n) = 3. Also a(n) = T(n,n-3), where T
 *          is the array defined in A024996.
 * https://oeis.org/A026070
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: compute the number of integer paths s(0)…s(n)
   with s(0)=0, |s(i)-s(i-1)|=1 for i=1,2, |s(i)-s(i-1)|≤1 for i≥3,
   and s(n)=3.  This straightforward DP runs in O(n²) time
   and O(n) memory, which is fast enough for n≤10000. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* For the OEIS offset the first term corresponds to n=3. */
    if (n < 3) return 0;

    /* Size of DP arrays: positions can range from -n to +n. */
    idx_t sz = 2 * n + 5;           /* extra padding to avoid bounds checks */
    size_t shift = n;                /* index of position 0 */

    /* Allocate two DP buffers. */
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!curr || !next) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(curr);
        free(next);
        return 0;
    }
    /* Initialize all entries to 0. */
    memset(curr, 0, sz * sizeof(seq_t));
    memset(next, 0, sz * sizeof(seq_t));

    /* Step 0: only one start position. */
    curr[shift + 0] = 1;

    /* Step 1: move exactly ±1. */
    next[shift + 1] = curr[shift + 0];
    next[shift - 1] = curr[shift + 0];
    /* Swap buffers. */
    seq_t *tmp = curr; curr = next; next = tmp;
    memset(next, 0, sz * sizeof(seq_t));

    /* Step 2: again move exactly ±1. */
    for (idx_t pos = 0; pos < sz; ++pos) {
        if (curr[pos] == 0) continue;
        if (pos > 0)          next[pos - 1] += curr[pos];
        if (pos + 1 < sz)     next[pos + 1] += curr[pos];
    }
    tmp = curr; curr = next; next = tmp;

    /* Steps 3 … n: allow steps of −1, 0, +1. */
    for (idx_t step = 3; step <= n; ++step) {
        memset(next, 0, sz * sizeof(seq_t));
        for (idx_t pos = 0; pos < sz; ++pos) {
            if (curr[pos] == 0) continue;
            if (pos > 0)          next[pos - 1] += curr[pos];
            next[pos]            += curr[pos];
            if (pos + 1 < sz)     next[pos + 1] += curr[pos];
        }
        tmp = curr; curr = next; next = tmp;
    }

    /* Result is the number of paths ending at position 3. */
    seq_t result = curr[shift + 3];

    free(curr);
    free(next);
    return result;
}
