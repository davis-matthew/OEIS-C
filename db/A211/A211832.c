/*
 * A211832  Number of nonnegative integer arrays of length n+5 with new values
 *          0 upwards introduced in order, no three adjacent elements equal,
 *          and containing the value 5.
 * https://oeis.org/A211832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of restricted-growth arrays of length n+5 with no three
 *            equal adjacent elements and containing the value 5. Computed by
 *            dynamic programming over (current maximum value, current run
 *            length, last value equals 5 or not, and seen-5 flag)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#if defined(__cplusplus)
#include <type_traits>
typedef typename std::conditional<
    (sizeof(seq_t) <= 8),
    unsigned __int128,
    seq_t>::type Big;
#else
typedef unsigned __int128 Big;
#endif

static inline size_t idx(size_t m, int run, int last5, int has5) {
    return m * 8 + (run - 1) * 4 + last5 * 2 + has5;
}

seq_t a(idx_t n) {
    size_t m = (size_t)n + 5;          /* array length */
    size_t sz = (m + 1) * 8;           /* number of DP states */

    Big *cur = (Big *)malloc(sz * sizeof(Big));
    Big *nxt = (Big *)malloc(sz * sizeof(Big));
    if (!cur || !nxt) {
        /* allocation failed */
        return (seq_t)0;
    }

    memset(cur, 0, sz * sizeof(Big));
    memset(nxt, 0, sz * sizeof(Big));

    /* initial state: first element is 0 */
    cur[idx(0, 1, 0, 0)] = 1;

    for (size_t pos = 1; pos < m; ++pos) {
        memset(nxt, 0, sz * sizeof(Big));
        size_t max_limit = pos;          /* current max can be at most pos-1 */
        for (size_t maxv = 0; maxv < max_limit; ++maxv) {
            for (int run = 1; run <= 2; ++run) {
                for (int last5 = 0; last5 <= 1; ++last5) {
                    for (int has5 = 0; has5 <= 1; ++has5) {
                        Big curval = cur[idx(maxv, run, last5, has5)];
                        if (!curval) continue;

                        /* 1) reuse the same value (increase run length) */
                        if (run == 1) {
                            nxt[idx(maxv, 2, last5, has5)] += curval;
                        }

                        /* 2) reuse a different existing value */
                        size_t diff_total = maxv;   /* number of values excluding last */
                        if (diff_total > 0) {
                            int have5opt = (last5 == 0 && maxv >= 5) ? 1 : 0;
                            size_t diff_non5 = diff_total - have5opt;
                            if (diff_non5 > 0) {
                                nxt[idx(maxv, 1, 0, has5)] += curval * (Big)diff_non5;
                            }
                            if (have5opt) {
                                nxt[idx(maxv, 1, 1, 1)] += curval;
                            }
                        }

                        /* 3) introduce a new value */
                        size_t newmax = maxv + 1;
                        int newlast5 = (newmax == 5) ? 1 : 0;
                        int newhas5 = has5 | newlast5;
                        nxt[idx(newmax, 1, newlast5, newhas5)] += curval;
                    }
                }
            }
        }
        /* swap cur and nxt */
        Big *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all states where the value 5 has appeared */
    Big result = 0;
    for (size_t maxv = 0; maxv < m; ++maxv) {
        for (int run = 1; run <= 2; ++run) {
            for (int last5 = 0; last5 <= 1; ++last5) {
                result += cur[idx(maxv, run, last5, 1)];
            }
        }
    }

    free(cur);
    free(nxt);

    return (seq_t)result;
}
