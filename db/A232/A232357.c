/*
 * A232357  The number of pairs of numbers below n that, when generating a
 *          Fibonacci-like sequence modulo n, do not contain zero.
 * https://oeis.org/A232357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of states in the Fibonacci‑mod‑n permutation cycle
 *            graph that do not contain any element with a zero component
 * Range:     u8 n <= 20, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   next_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_idx.h"

static inline bool has_zero(uint32_t idx, uint32_t n) {
    return (idx / n == 0) || (idx % n == 0);
}

seq_t a(idx_t n)
{
    uint64_t N = n;
    if (N == 0) return 0;          /* safety: n ≥ 1 in the OEIS offset   */
    uint64_t T = N * N;             /* total number of states (x,y), n ≤ 10000 → T ≤ 1e8 */
    uint32_t n32 = (uint32_t)N;     /* fits in 32 bits because n ≤ 10000       */
    uint8_t *status = (uint8_t*)calloc((size_t)T, 1);  /* 0 = unknown, 1 = visited, 0xFF = current cycle */
    if (!status) return 0;          /* allocation failure                           */

    uint64_t bad = 0;               /* number of states whose cycle contains a zero component */

    for (uint64_t ii = 0; ii < T; ++ii) {
        if (status[ii]) continue;   /* already processed */

        uint32_t start = (uint32_t)ii;
        uint32_t cur = start;

        /* Mark all nodes in the current cycle temporarily with 0xFF */
        do {
            status[cur] = 0xFF;
            cur = next_idx(cur, n32);
        } while (cur != start);

        /* Second pass: count cycle length, detect zeros, and mark as visited */
        uint64_t cycle_len = 0;
        bool has_zero_here = false;
        cur = start;
        do {
            if (has_zero(cur, n32)) has_zero_here = true;
            status[cur] = 1;          /* mark as visited */
            cycle_len++;
            cur = next_idx(cur, n32);
        } while (cur != start);

        if (has_zero_here) bad += cycle_len;
    }

    uint64_t good = T - bad;       /* a(n) = good states */
    free(status);
    return (seq_t)good;
}
