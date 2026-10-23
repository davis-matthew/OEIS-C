/*
 * A143282  Number of binary words of length n containing at least one subword
 *          1001 and no subwords 10^{i}1 with i<2.
 * https://oeis.org/A143282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (# binary strings of length n avoiding 11 and 101) *
 *            minus (# binary strings of length n avoiding 11, 101 and 1001)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* transition tables
 * state indices: 0 = "", 1 = "1", 2 = "10", 3 = "100"
 * values < 0 mean the transition leads to a forbidden pattern
 */
static const signed char trans_full[4][2] = {
    { 0,  1},   /* '' -> '' on 0, '' -> '1' on 1 */
    { 2, -1},   /* '1' -> '10' on 0, '1' -> forbidden on 1 */
    { 3, -1},   /* '10' -> '100' on 0, '10' -> forbidden on 1 */
    { 0, -1}    /* '100' -> '' on 0, '100' -> forbidden on 1 */
};

static const signed char trans_part[4][2] = {
    { 0,  1},
    { 2, -1},
    { 3, -1},
    { 0,  1}    /* '100' -> '' on 0, '100' -> '1' on 1 */
};

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cnt_part = NULL;     /* counts avoiding 11 & 101 */
    static seq_t *cnt_full = NULL;     /* counts avoiding 11, 101 & 1001 */
    static size_t    cap    = 0;       /* current allocated capacity */
    static size_t    cur_len= 0;       /* longest length already computed */
    static seq_t dp_p[4], nd_p[4];     /* DP arrays for part */
    static seq_t dp_f[4], nd_f[4];     /* DP arrays for full */

    /* initialise on first call */
    if (cnt_part == NULL) {
        cap = 1;
        cnt_part = (seq_t *)malloc(cap * sizeof(seq_t));
        cnt_full = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cnt_part || !cnt_full) abort();

        /* start with length 0 */
        for (int i = 0; i < 4; ++i) dp_p[i] = dp_f[i] = 0;
        dp_p[0] = dp_f[0] = 1;          /* only empty string */
        cnt_part[0] = 1;
        cnt_full[0] = 1;
    }

    /* extend precomputation up to required n */
    while (cur_len < n) {
        /* -- compute next length for avoiding 11 & 101 -- */
        for (int i = 0; i < 4; ++i) nd_p[i] = 0;
        for (int s = 0; s < 4; ++s) {
            seq_t val = dp_p[s];
            if (!val) continue;
            for (int b = 0; b < 2; ++b) {
                signed char ns = trans_part[s][b];
                if (ns >= 0) nd_p[(int)ns] += val;
            }
        }
        for (int i = 0; i < 4; ++i) dp_p[i] = nd_p[i];
        seq_t sum_p = 0;
        for (int i = 0; i < 4; ++i) sum_p += dp_p[i];
        ++cur_len;
        if (cur_len >= cap) {
            cap <<= 1;
            cnt_part = (seq_t *)realloc(cnt_part, cap * sizeof(seq_t));
            cnt_full = (seq_t *)realloc(cnt_full, cap * sizeof(seq_t));
            if (!cnt_part || !cnt_full) abort();
        }
        cnt_part[cur_len] = sum_p;

        /* -- compute next length for avoiding 11, 101 and 1001 -- */
        for (int i = 0; i < 4; ++i) nd_f[i] = 0;
        for (int s = 0; s < 4; ++s) {
            seq_t val = dp_f[s];
            if (!val) continue;
            for (int b = 0; b < 2; ++b) {
                signed char ns = trans_full[s][b];
                if (ns >= 0) nd_f[(int)ns] += val;
            }
        }
        for (int i = 0; i < 4; ++i) dp_f[i] = nd_f[i];
        seq_t sum_f = 0;
        for (int i = 0; i < 4; ++i) sum_f += dp_f[i];
        cnt_full[cur_len] = sum_f;
    }

    size_t idx = (size_t)n;
    return cnt_part[idx] - cnt_full[idx];
}
