/*
 * A222353  Number of nX1 0..1 arrays with no more than floor(nX1/2) elements
 *          unequal to at least one king-move neighbor, with new values
 *          introduced in row major 0..1 order.
 * https://oeis.org/A222353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 39, u64 n <= 76, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS A222353:
   the number of binary n×1 arrays with no more than ⌊n/2⌋ cells
   that are unequal to at least one of their king‑move neighbors.
   The values are computed with a dynamic programming that keeps,
   for each last two bits, the maximum allowed number of unequal
   cells seen so far.  The code is written to compile as both C11
   and C++17; a std::vector<seq_t> is used in C++ builds to ensure
   constructors/destructors are invoked for arbitrary‑precision
   types. */

#include <stdlib.h>   /* malloc, free, calloc, memset for C only */
#include <string.h>   /* memset */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* offsets: a(1)=1, a(2)=1, a(3)=1 as given */
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 1;

    idx_t maxDiff = n / 2;
    size_t W = (size_t)maxDiff + 1;
    size_t SZ = 4 * W;          /* 4 = 2^2 possible last‑two‑bit states */

#ifdef __cplusplus
    std::vector<seq_t> prev_vec(SZ), cur_vec(SZ);
    seq_t *prev = prev_vec.data();
    seq_t *cur  = cur_vec.data();
#else
    seq_t *prev = (seq_t*)calloc(SZ, sizeof(seq_t));
    seq_t *cur  = (seq_t*)calloc(SZ, sizeof(seq_t));
    if (!prev || !cur) return 0;    /* allocation failed */
#endif

    /* Initial state after fixing first cell to 0 and choosing second cell */
    for (int bit = 0; bit <= 1; ++bit) {
        int d = (bit != 0) ? 1 : 0;      /* cell 1 is unequal if bit=1 */
        size_t idx = (size_t)bit * W + d;
        prev[idx] = 1;
    }

    /* DP for positions 3 .. n */
    for (idx_t pos = 3; pos <= n; ++pos) {
#ifdef __cplusplus
        std::fill_n(cur, SZ, 0);
#else
        memset(cur, 0, SZ * sizeof(seq_t));
#endif
        for (int idx = 0; idx < 4; ++idx) {
            int prev1 = idx >> 1;       /* cell pos-2 */
            int prev2 = idx & 1;        /* cell pos-1 */
            for (idx_t d_seen = 0; d_seen <= maxDiff; ++d_seen) {
                seq_t val = prev[(size_t)idx * W + d_seen];
                if (!val) continue;
                for (int curbit = 0; curbit <= 1; ++curbit) {
                    /* The (pos-1) cell is counted if it differs from
                       either its left neighbor (prev1) or right neighbor (curbit). */
                    int add = ((prev1 != prev2) || (prev2 != curbit)) ? 1 : 0;
                    idx_t nd = d_seen + add;
                    if (nd > maxDiff) continue;
                    int nidx = (prev2 << 1) | curbit;
                    cur[(size_t)nidx * W + nd] += val;
                }
            }
        }
        /* swap pointers/layers */
        seq_t *tmp = prev;  prev = cur;  cur = tmp;
    }

    /* Final step: account for the last cell (n) */
    seq_t result = 0;
    for (int idx = 0; idx < 4; ++idx) {
        int prev1 = idx >> 1;   /* cell n-2 */
        int prev2 = idx & 1;    /* cell n-1 */
        int last_diff = (prev1 != prev2) ? 1 : 0;
        for (idx_t d_seen = 0; d_seen <= maxDiff; ++d_seen) {
            seq_t val = prev[(size_t)idx * W + d_seen];
            if (!val) continue;
            idx_t total = d_seen + last_diff;
            if (total <= maxDiff) result += val;
        }
    }

#ifndef __cplusplus
    free(prev);
    free(cur);
#endif
    return result;
}
