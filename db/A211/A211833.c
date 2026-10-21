/*
 * A211833  Number of nonnegative integer arrays of length n+6 with new values
 *          0 upwards introduced in order, no three adjacent elements equal,
 *          and containing the value 6.
 * https://oeis.org/A211833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A211833
    a(n) = number of non‑negative integer arrays of length n+6
    with new values introduced in order, no three adjacent elements equal,
    and containing the value 6.
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *alloc_seq(size_t sz) {
#ifdef __cplusplus
    /* In C++: allocate objects with constructor */
    seq_t *p = new seq_t[sz];
#else
    /* In C: plain allocation of POD type */
    seq_t *p = (seq_t*)malloc(sz * sizeof(seq_t));
#endif
    /* initialise to zero */
    for (size_t i = 0; i < sz; ++i) p[i] = 0;
    return p;
}

static void free_seq(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n) {
    /* total array length */
    size_t N = (size_t)n + 6;
    /* each state is encoded as 8 variations:
       bits: [seen6][last6][r-1]  (r=1 -> 0, r=2 -> 1) */
    size_t arraySize = (N + 2) * 8;          /* maximum number of states needed */

    seq_t *cur  = alloc_seq(arraySize);
    seq_t *next = alloc_seq(arraySize);

    /* initial state: length = 1, m = 0, r = 1, last = 0, seen6 = 0 */
    cur[0] = 1;

    for (size_t pos = 1; pos < N; ++pos) {   /* build positions 2 .. N */
        size_t curMaxM = pos - 1;            /* current maximum m */
        size_t nextMaxM = pos;               /* new maximum m after adding element */
        size_t nextSize = (nextMaxM + 1) * 8;

        /* clear next buffer for indices that will be used */
        for (size_t i = 0; i < nextSize; ++i) next[i] = 0;

        for (size_t m = 0; m <= curMaxM; ++m) {
            size_t base = m * 8;
            for (int encode = 0; encode < 8; ++encode) {
                seq_t cnt = cur[base + encode];
                if (cnt == 0) continue;

                int seen6 = encode & 1;
                int last6 = (encode >> 1) & 1;
                int rflag = (encode >> 2) & 1;
                int r = rflag + 1;            /* current run length (1 or 2) */

                /* 1) choose a new value (m+1) */
                size_t new_m = m + 1;
                int new_last6 = (new_m == 6) ? 1 : 0;
                int new_seen6 = seen6 | new_last6;
                int new_enc_new = (0 << 2) | (new_last6 << 1) | new_seen6; /* r = 1 */
                next[new_m * 8 + new_enc_new] += cnt;

                /* 2) repeat the current last value (run length increases) */
                if (r == 1) {
                    int new_enc_same = (1 << 2) | (last6 << 1) | seen6; /* r = 2 */
                    next[base + new_enc_same] += cnt;
                }

                /* 3) pick a different previous value (not equal to last) */
                int other_count = (int)m;   /* number of previous values excluding last */

                if (other_count > 0) {
                    if (m >= 6 && last6 == 0) {
                        /* choose value 6 as the new last value */
                        int enc6 = (0 << 2) | (1 << 1) | 1;          /* last6=1, seen6=1 */
                        next[base + enc6] += cnt;

                        /* choose any other previous value (not 6) */
                        int rem = m - 1;
                        if (rem > 0) {
                            seq_t add = cnt * (seq_t)rem;
                            int enc_other = (0 << 2) | (last6 << 1) | seen6;
                            next[base + enc_other] += add;
                        }
                    } else {
                        /* all other previous values are not 6 */
                        seq_t add = cnt * (seq_t)other_count;
                        int enc_other = (0 << 2) | (last6 << 1) | seen6;
                        next[base + enc_other] += add;
                    }
                }
            }
        }

        /* swap buffers for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum over all states with seen6 == 1 */
    seq_t ans = 0;
    for (size_t m = 0; m <= N - 1; ++m) {
        size_t base = m * 8;
        for (int encode = 0; encode < 8; ++encode) {
            if (encode & 1) ans += cur[base + encode];
        }
    }

    free_seq(cur);
    free_seq(next);
    return ans;
}
