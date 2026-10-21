/*
 * A232052  Number of 6Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n 0..1 arrays in which no element is less
 *            than * a strict majority of its horizontal and anti‑diagonal
 *            neighbors. * The value is computed by dynamic programming over
 *            pairs of columns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline bool col_ok(uint64_t curr, bool has_prev, bool has_next,
                          uint64_t prev, uint64_t next)
{
    for (int r = 0; r < 6; ++r) {
        int neigh = 0;
        int ones  = 0;

        /* left horizontal */
        if (has_prev) {
            ++neigh;
            if ((prev >> r) & 1U) ++ones;
        }
        /* right horizontal */
        if (has_next) {
            ++neigh;
            if ((next >> r) & 1U) ++ones;
        }
        /* antidiagonal up: next row r-1 */
        if (has_next && r > 0) {
            ++neigh;
            if ((next >> (r - 1)) & 1U) ++ones;
        }
        /* antidiagonal down: prev row r+1 */
        if (has_prev && r < 5) {
            ++neigh;
            if ((prev >> (r + 1)) & 1U) ++ones;
        }

        /* if current cell is 0, it must not be less than a strict majority
         * of its neighbors (i.e., ones must be <= floor(neigh/2)). */
        if (((curr >> r) & 1U) == 0) {
            int allowed = neigh / 2;
            if (ones > allowed)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Small n cases */
    if (n == 0)                    /* unused, but safe */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)64;          /* 2^6 */

    /* Pre‑compute transitions: for each (prev,curr) the list of next columns
     * that satisfy the constraint for curr using both neighbors.  The
     * number of states is 64×64 = 4096. */
    static int (*next_list)[64] = NULL;
    static int *next_count = NULL;
    if (!next_list) {
        next_list = (int (*)[64])malloc(4096 * 64 * sizeof(int));
        next_count = (int *)malloc(4096 * sizeof(int));
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                int idx = (prev << 6) | curr;
                int cnt = 0;
                for (int next = 0; next < 64; ++next) {
                    if (col_ok((uint64_t)curr, true, true,
                               (uint64_t)prev, (uint64_t)next)) {
                        next_list[idx][cnt++] = next;
                    }
                }
                next_count[idx] = cnt;
            }
        }
    }

    /* DP arrays for counts of pairs (prev,curr). */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    if (!dp_cur) {
        dp_cur  = (seq_t *)malloc(4096 * sizeof(seq_t));
        dp_next = (seq_t *)malloc(4096 * sizeof(seq_t));
    }

    /* n == 2 ----------------------------------------------------------- */
    if (n == 2) {
        seq_t total = 0;
        for (int c1 = 0; c1 < 64; ++c1) {
            for (int c2 = 0; c2 < 64; ++c2) {
                if (!col_ok((uint64_t)c1, false, true, 0, (uint64_t)c2))
                    continue;
                if (!col_ok((uint64_t)c2, true, false, (uint64_t)c1, 0))
                    continue;
                total += (seq_t)1;
            }
        }
        return total;
    }

    /* n >= 3 ----------------------------------------------------------- */
    /* initialize dp_cur with all pairs (c1,c2) where column 1 is valid
     * with has_prev=0, has_next=1. */
    for (int i = 0; i < 4096; ++i) dp_cur[i] = (seq_t)0;
    for (int c1 = 0; c1 < 64; ++c1) {
        for (int c2 = 0; c2 < 64; ++c2) {
            if (col_ok((uint64_t)c1, false, true, 0, (uint64_t)c2))
                dp_cur[(c1 << 6) | c2] = (seq_t)1;
        }
    }

    /* iterate columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 4096; ++i) dp_next[i] = (seq_t)0;

        for (int idx = 0; idx < 4096; ++idx) {
            seq_t ways = dp_cur[idx];
            if (ways == (seq_t)0) continue;

            int prev = idx >> 6;
            int curr = idx & 63;
            int cnt  = next_count[idx];
            int *list = next_list[idx];
            for (int k = 0; k < cnt; ++k) {
                int next = list[k];
                dp_next[(curr << 6) | next] += ways;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* finalize: check column n with has_next=false */
    seq_t total = 0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t ways = dp_cur[idx];
        if (ways == (seq_t)0) continue;
        int prev = idx >> 6;
        int curr = idx & 63;
        if (col_ok((uint64_t)curr, true, false,
                   (uint64_t)prev, 0U)) {
            total += ways;
        }
    }
    return total;
}
