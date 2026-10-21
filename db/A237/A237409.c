/*
 * A237409  Number of (n+1) X (5+1) 0..2 arrays with the maximum plus the
 *          upper median plus the minimum of every 2 X 2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×6 0..2 arrays whose 2×2 subblocks' f values
 *            (min + upper median + max) form an n×5 grid with absolute
 *            difference 1 between all horizontal and vertical neighbor cells
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   block_f, encode_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------ helpers ------------------------------------*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/block_f.h"
#include "../utils/encode_sub.h"

/* row of 6 values in 0..2 */
static uint8_t rows[729][6];

/* check horizontal validity of length‑5 subblock values (difference ±1) */
static inline bool horiz_valid(const uint8_t v[5]) {
    for (int i = 0; i < 4; ++i) {
        int d = v[i] - v[i + 1];
        if (d != 1 && d != -1) return false;
    }
    return true;
}

/*--------------------------------- main routine ---------------------------------*/

seq_t a(idx_t n) {
    static bool inited = false;

    /*--------------------------- static workspace ---------------------------*/
    static int sub_id[16807] = {0};
    static int K = 0;                        /* number of distinct subblocks */
    static uint8_t (*sub_rows)[5] = NULL;    /* mapping sub_id -> array of 5 values */

    static int pair_count = 0;
    static int *pair_a = NULL;
    static int *pair_b = NULL;
    static int *pair_sid = NULL;
    static int *next_idx = NULL;             /* linked list of pairs by first row */
    static int head[729] = {0};

    static bool (*valid_trans)[74] = NULL;    /* (max K=74) */

    static seq_t *dp_prev = NULL;
    static seq_t *dp_curr = NULL;

    static seq_t *seq_val = NULL;            /* results cache a(1) .. a(max_n) */
    static idx_t cached_max = 0;             /* highest n computed so far */

    /*------------------------- initialization --------------------------*/
    if (!inited) {
        inited = true;

        /* generate all 6‑cell rows (base-3). */
        for (int code = 0; code < 729; ++code) {
            int t = code;
            for (int i = 5; i >= 0; --i) {
                rows[code][i] = (uint8_t)(t % 3);
                t /= 3;
            }
        }

        /* initialise mapping from encoded subblock to id */
        for (int i = 0; i < 16807; ++i) sub_id[i] = -1;

        /* enumerate all horizontally valid subblock rows */
        for (int s0 = 0; s0 < 7; ++s0)
            for (int s1 = 0; s1 < 7; ++s1)
                for (int s2 = 0; s2 < 7; ++s2)
                    for (int s3 = 0; s3 < 7; ++s3)
                        for (int s4 = 0; s4 < 7; ++s4) {
                            uint8_t v[5] = { (uint8_t)s0, (uint8_t)s1,
                                             (uint8_t)s2, (uint8_t)s3,
                                             (uint8_t)s4 };
                            if (!horiz_valid(v)) continue;
                            int code = encode_sub(v);
                            if (sub_id[code] == -1) sub_id[code] = K++;
                        }

        sub_rows = (uint8_t (*)[5])malloc(K * 5 * sizeof(uint8_t));
        for (int s0 = 0; s0 < 7; ++s0)
            for (int s1 = 0; s1 < 7; ++s1)
                for (int s2 = 0; s2 < 7; ++s2)
                    for (int s3 = 0; s3 < 7; ++s3)
                        for (int s4 = 0; s4 < 7; ++s4) {
                            uint8_t v[5] = { (uint8_t)s0, (uint8_t)s1,
                                             (uint8_t)s2, (uint8_t)s3,
                                             (uint8_t)s4 };
                            if (!horiz_valid(v)) continue;
                            int code = encode_sub(v);
                            int id = sub_id[code];
                            for (int i = 0; i < 5; ++i)
                                sub_rows[id][i] = v[i];
                        }

        /* Count all valid (a,b) pairs and collect them */
        int temp_pair_count = 0;
        for (int a = 0; a < 729; ++a)
            for (int b = 0; b < 729; ++b) {
                uint8_t s[5];
                for (int c = 0; c < 5; ++c)
                    s[c] = block_f(rows[a][c], rows[a][c + 1],
                                    rows[b][c], rows[b][c + 1]);
                if (!horiz_valid(s)) continue;
                ++temp_pair_count;
            }
        pair_count = temp_pair_count;

        pair_a = (int *)malloc(pair_count * sizeof(int));
        pair_b = (int *)malloc(pair_count * sizeof(int));
        pair_sid = (int *)malloc(pair_count * sizeof(int));
        next_idx = (int *)malloc(pair_count * sizeof(int));
        for (int i = 0; i < 729; ++i) head[i] = -1;

        /* second pass: fill pair arrays and build linked lists */
        int idx = 0;
        for (int a = 0; a < 729; ++a) {
            for (int b = 0; b < 729; ++b) {
                uint8_t s[5];
                for (int c = 0; c < 5; ++c)
                    s[c] = block_f(rows[a][c], rows[a][c + 1],
                                    rows[b][c], rows[b][c + 1]);
                if (!horiz_valid(s)) continue;
                int code = encode_sub(s);
                int sid = sub_id[code];
                pair_a[idx] = a;
                pair_b[idx] = b;
                pair_sid[idx] = sid;
                next_idx[idx] = head[a];
                head[a] = idx;
                ++idx;
            }
        }

        /* vertical adjacency between subblocks */
        valid_trans = (bool (*)[74])malloc(K * K * sizeof(bool));
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j) {
                bool ok = true;
                for (int k = 0; k < 5; ++k) {
                    int d = sub_rows[i][k] - sub_rows[j][k];
                    if (d < 0) d = -d;
                    if (d != 1) { ok = false; break; }
                }
                valid_trans[i][j] = ok;
            }

        dp_prev = (seq_t *)malloc(pair_count * sizeof(seq_t));
        dp_curr = (seq_t *)malloc(pair_count * sizeof(seq_t));
        for (int i = 0; i < pair_count; ++i) dp_prev[i] = 1; /* step 1 */

        seq_val = (seq_t *)malloc(sizeof(seq_t));
        seq_val[0] = (seq_t)pair_count; /* a(1) */
        cached_max = 1;
    }

    /*----------------------- extend cache if needed -----------------------*/
    while (cached_max < n) {
        /* perform one DP step : add a new row */
        for (int i = 0; i < pair_count; ++i) dp_curr[i] = 0;

        for (int i = 0; i < pair_count; ++i) {
            seq_t val = dp_prev[i];
            if (val == (seq_t)0) continue;
            int cur_b = pair_b[i];
            int cur_sid = pair_sid[i];
            int next = head[cur_b];
            while (next != -1) {
                int j = next;
                if (valid_trans[cur_sid][pair_sid[j]]) {
                    dp_curr[j] += val;
                }
                next = next_idx[j];
            }
        }

        /* swap dp arrays */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;

        /* compute total for this n */
        seq_t total = 0;
        for (int i = 0; i < pair_count; ++i) total += dp_prev[i];

        /* grow seq_val if necessary */
        idx_t new_size = cached_max + 1;
        seq_val = (seq_t *)realloc(seq_val, new_size * sizeof(seq_t));
        seq_val[cached_max] = total;
        ++cached_max;
    }

    return seq_val[n - 1];
}
