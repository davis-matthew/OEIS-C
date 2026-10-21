/*
 * A228682  Number of nX7 binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with no two 1’s adjacent
 *            horizontally, diagonally, or antidiagonally. Computation:
 *            dynamic programming across 7‑bit row masks obeying the adjacency
 *            rules. Uses memoisation so calls may arrive in any order
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* state variables ---------------------------------------------------- */

static idx_t num_states = 0;            /* number of valid 7‑bit masks (34) */
static idx_t *valid_mask = NULL;        /* array of masks */
static size_t *compat_cnt = NULL;       /* compatibility count per mask */
static size_t *compat_off = NULL;       /* offset into the flat compat list */
static idx_t *compat_list = NULL;       /* flat list of compatible indices */

/* DP buffers */
static seq_t *dp_cur = NULL;            /* values for current n */
static seq_t *dp_nxt = NULL;            /* values for next n */

/* cache of sequence values (1‑based index) */
static seq_t *cache_vals = NULL;
static idx_t cache_len = 0;             /* allocated size */
static idx_t cache_computed = 0;        /* largest n for which cache_vals contains a(n) */

/* last n processed (len of dp_cur) */
static idx_t last_n = 0;

/* initialise all lookup tables and DP buffers ----------------------------------- */
static void init_state(void) {
    static bool finished = false;
    if (finished) return;
    finished = true;

    /* find all 7‑bit masks with no horizontally adjacent 1‑bits */
    for (idx_t mask = 0; mask < (1u << 7); ++mask) {
        if ((mask & (mask << 1)) == 0) ++num_states;
    }
    valid_mask = (idx_t *)malloc(num_states * sizeof(idx_t));
    if (!valid_mask) abort();

    for (idx_t mask = 0, k = 0; mask < (1u << 7); ++mask) {
        if ((mask & (mask << 1)) == 0) valid_mask[k++] = mask;
    }

    /* allocate compatibility structures */
    compat_cnt  = (size_t *)malloc(num_states * sizeof(size_t));
    compat_off  = (size_t *)malloc(num_states * sizeof(size_t));
    compat_list = (idx_t *)malloc(num_states * num_states * sizeof(idx_t));
    if (!compat_cnt || !compat_off || !compat_list) abort();

    /* compute compatibility counts */
    for (size_t i = 0; i < num_states; ++i) {
        size_t cnt = 0;
        for (size_t j = 0; j < num_states; ++j) {
            idx_t m1 = valid_mask[i];
            idx_t m2 = valid_mask[j];
            if ((m1 & ((m2 << 1) | (m2 >> 1))) == 0) ++cnt;
        }
        compat_cnt[i] = cnt;
    }

    /* compute offsets for the flat list */
    size_t accum = 0;
    for (size_t i = 0; i < num_states; ++i) {
        compat_off[i] = accum;
        accum += compat_cnt[i];
    }

    /* fill the flat list */
    size_t *temp_cnt = (size_t *)calloc(num_states, sizeof(size_t));
    for (size_t i = 0; i < num_states; ++i) {
        for (size_t j = 0; j < num_states; ++j) {
            idx_t m1 = valid_mask[i];
            idx_t m2 = valid_mask[j];
            if ((m1 & ((m2 << 1) | (m2 >> 1))) == 0) {
                size_t pos = compat_off[i] + temp_cnt[i];
                compat_list[pos] = (idx_t)j;
                ++temp_cnt[i];
            }
        }
    }
    free(temp_cnt);

    /* allocate DP buffers */
    dp_cur = (seq_t *)malloc(num_states * sizeof(seq_t));
    dp_nxt = (seq_t *)malloc(num_states * sizeof(seq_t));
    if (!dp_cur || !dp_nxt) abort();

    /* initialise DP for n = 1: each valid mask occurs exactly once */
    for (size_t i = 0; i < num_states; ++i) dp_cur[i] = (seq_t)1;

    /* compute a(1) = number of valid masks */
    seq_t total = 0;
    for (size_t i = 0; i < num_states; ++i) total += dp_cur[i];

    /* initialise cache */
    cache_len = 1;
    cache_vals = (seq_t *)malloc(sizeof(seq_t));
    if (!cache_vals) abort();
    cache_vals[0] = total;

    cache_computed = 1;
    last_n = 1;
}

/* --------------------------------------------------------------------- */
/* compute a(n) ------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    init_state();                 /* ensure tables are built */

    if (n > cache_len) {          /* grow cache if needed */
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache_vals, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache_vals = tmp;
        cache_len = new_len;
    }

    if (n > cache_computed) {     /* generate new terms up to n */
        for (idx_t step = cache_computed + 1; step <= n; ++step) {
            /* compute dp_nxt[mask] = sum over compatible previous masks */
            for (size_t i = 0; i < num_states; ++i) {
                seq_t sum = 0;
                size_t off = compat_off[i];
                size_t cnt = compat_cnt[i];
                for (size_t k = 0; k < cnt; ++k) {
                    sum += dp_cur[compat_list[off + k]];
                }
                dp_nxt[i] = sum;
            }

            /* total number of arrays for this n */
            seq_t tot = 0;
            for (size_t i = 0; i < num_states; ++i) tot += dp_nxt[i];
            cache_vals[step - 1] = tot;

            /* swap DP buffers for next iteration */
            seq_t *tmp_buf = dp_cur;
            dp_cur = dp_nxt;
            dp_nxt = tmp_buf;
        }
        cache_computed = n;
        last_n = n;
    }

    return cache_vals[n - 1];
}
