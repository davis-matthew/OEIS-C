/*
 * A207696  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays with no horizontal triplets
 *            000 or 010 and no vertical triplets 001 or 111. Computed by
 *            dynamic programming over column states. Initial values: a(1)=13,
 *            a(2)=169
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data kept between calls */
    static bool initialized = false;
    static size_t SIZE_S;                /* number of allowed columns  (13) */
    static size_t M;                     /* SIZE_S * SIZE_S  (169) */
    static int   *trans;                 /* transition tables  M * SIZE_S  */
    static unsigned char *outdeg;        /* number of transitions per state  M */
    static seq_t *cur, *next;            /* DP arrays for current / next step */
    static seq_t *ans;                   /* array of computed terms */
    static size_t ans_len;               /* number of terms stored in ans   */
    static size_t max_done;              /* largest n for which terms are known */

    if (!initialized) {
        /* ----------  compute allowed columns  ---------- */
        int allowed[32];
        size_t allowed_cnt = 0;
        for (int m = 0; m < 32; ++m) {
            int bits[5];
            for (int r = 0; r < 5; ++r) bits[r] = (m >> r) & 1;
            bool bad = false;
            for (int i = 0; i < 3; ++i) {
                /* vertical patterns 111 and 001 are forbidden */
                if (bits[i]    && bits[i+1] && bits[i+2]) { bad = true; break; }
                if (!bits[i] && !bits[i+1] &&  bits[i+2]) { bad = true; break; }
            }
            if (!bad) allowed[allowed_cnt++] = m;
        }
        SIZE_S = allowed_cnt;            /* should be 13 */
        M = SIZE_S * SIZE_S;             /* 169 */

        /* ----------  allocate transition tables  ---------- */
        trans  = (int*)      malloc(M * SIZE_S * sizeof(int));
        outdeg = (unsigned char*)malloc(M * sizeof(unsigned char));

        /* ----------  build transition tables  ---------- */
        for (size_t a = 0; a < SIZE_S; ++a) {
            for (size_t b = 0; b < SIZE_S; ++b) {
                size_t idx = a * SIZE_S + b;
                unsigned char deg = 0;
                int mask_a = allowed[a];
                int mask_b = allowed[b];
                for (size_t c = 0; c < SIZE_S; ++c) {
                    int mask_c = allowed[c];
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {
                        int a_bit = (mask_a >> r) & 1;
                        int b_bit = (mask_b >> r) & 1;
                        int c_bit = (mask_c >> r) & 1;
                        if (a_bit==0 && b_bit==0 && c_bit==0) { ok = false; break; }
                        if (a_bit==0 && b_bit==1 && c_bit==0) { ok = false; break; }
                    }
                    if (ok) {
                        trans[idx*SIZE_S + deg] = (int)c;
                        deg++;
                    }
                }
                outdeg[idx] = deg;
            }
        }

        /* ----------  allocate DP arrays  ---------- */
        cur  = (seq_t*)malloc(M * sizeof(seq_t));
        next = (seq_t*)malloc(M * sizeof(seq_t));

        /* ----------  DP for n = 2  (all states count as 1)  ---------- */
        for (size_t i = 0; i < M; ++i) cur[i] = (seq_t)1;

        /* ----------  store initial terms  ---------- */
        ans_len = 2;                     /* terms 1 and 2 */
        ans = (seq_t*)malloc(ans_len * sizeof(seq_t));
        ans[0] = (seq_t)SIZE_S;
        ans[1] = (seq_t)SIZE_S * (seq_t)SIZE_S;

        max_done = 2;
        initialized = true;
    }

    /* ----------  answer if already known  ---------- */
    if ((size_t)n <= ans_len) {
        return ans[n-1];
    }

    /* ----------  extend DP up to requested n  ---------- */
    for (size_t step = max_done + 1; step <= (size_t)n; ++step) {
        /* zero next array */
        for (size_t i = 0; i < M; ++i) next[i] = (seq_t)0;

        /* propagate transitions */
        for (size_t idx = 0; idx < M; ++idx) {
            seq_t val = cur[idx];
            if (val == (seq_t)0) continue;
            unsigned char deg = outdeg[idx];
            int base = idx * SIZE_S;
            size_t b = idx % SIZE_S;            /* previous column index */
            for (int k = 0; k < deg; ++k) {
                int c = trans[base + k];
                size_t new_idx = b * SIZE_S + (size_t)c;
                next[new_idx] += val;
            }
        }

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < M; ++i) total += next[i];

        /* append to answer array */
        ans = (seq_t*)realloc(ans, (ans_len + 1) * sizeof(seq_t));
        ans[ans_len] = total;
        ++ans_len;

        /* swap current and next */
        seq_t *tmp = cur;
        cur  = next;
        next = tmp;
    }

    max_done = (size_t)n;
    return ans[n-1];
}
