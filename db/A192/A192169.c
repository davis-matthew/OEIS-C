/*
 * A192169  Monotonic ordering of nonnegative differences 4^i-9^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th smallest nonnegative value of 4^i - 9^j
   with 0 ≤ i ≤ 40 and j ≥ 0.  The sequence is obtained by
   generating all such differences and sorting them in ascending
   order. */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* Static cache for the generated and sorted differences. */
    static seq_t cache[600];
    static size_t cached = 0;
    if (cached == 0) {
        /* Pre‑compute powers of 4 and 9 using 128‑bit integers
           to avoid overflow when seq_t is 64‑bit or less. */
        __int128 pow4[41];
        pow4[0] = 1;
        for (int i = 1; i <= 40; ++i) {
            pow4[i] = pow4[i-1] * 4;
        }
        /* 9^j grows more slowly; 9^30 < 2^128, so a small table suffices. */
        __int128 pow9[31];
        pow9[0] = 1;
        for (int j = 1; j <= 30; ++j) {
            pow9[j] = pow9[j-1] * 9;
        }

        /* Count bits needed to represent UINT64_MAX in the 128‑bit type. */
        const __int128 UINT64_MAX128 = (__int128)~(uint64_t)0;

        size_t cnt = 0;
        for (int i = 0; i <= 40; ++i) {
            __int128 pf = pow4[i];
            for (int j = 0; j <= 30; ++j) {
                __int128 pj = pow9[j];
                if (pj > pf) break;
                __int128 diff = pf - pj;
                /* Skip values that do not fit into seq_t when it is 64‑bit
                   or smaller; otherwise keep all values. */
                if (sizeof(seq_t) <= 8 && diff > UINT64_MAX128) {
                    continue;
                }
                if (cnt < sizeof(cache)/sizeof(cache[0])) {
                    cache[cnt++] = (seq_t)diff;
                }
            }
        }
        /* Now sort the cache. */
        qsort(cache, cnt, sizeof(seq_t), cmp_seq);
        cached = cnt;
    }
    /* Bounds check: if n exceeds the number of generated terms,
       return the last term (sequence is finite). */
    if (n == 0 || n > cached) {
        return cache[cached - 1];
    }
    return cache[n - 1];
}
