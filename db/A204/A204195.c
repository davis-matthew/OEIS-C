/*
 * A204195  Number of n X 6 0..1 arrays with no occurrence of three equal
 *          elements in a row horizontally, vertically or nw-to-se diagonally,
 *          and new values 0..1 introduced in row major order.
 * https://oeis.org/A204195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with no occurrence of three
 *            equal elements horizontally, vertically or NW‑to‑SE diagonally,
 *            with the first cell fixed to 0. Computed by dynamic programming
 *            over rows. a(1) = 13, a(2) = 338, a(3) = 639,
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   good_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/good_transition.h"

static unsigned char valid_rows[64];           /* all 6‑bit rows that avoid 000 or 111 */
static int m;                                 /* number of valid rows */
static int m0;                                /* number of valid rows with bit0 = 0 */

/* Transitions: for each state (i2,i1) we store the list of i3 that can
   follow.  The state index is i2*m + i1.  Since m ≤ 26 we can afford
   static arrays of size 64*64 = 4096 and each with up to 64 successors. */
#define MAXC 64
static unsigned char trans_to[MAXC * MAXC][MAXC];
static unsigned char trans_len[MAXC * MAXC];

/* dynamic programming tables */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static size_t dp_size = 0;   /* m*m */

/* caching of results */
static seq_t *results = NULL;
static size_t results_cap = 0;
static idx_t cache_max_n = 0;
static int init_done = 0;

/* helper to count if a 6‑bit pattern has no 000 or 111 horizontally */
static bool good_row(int r)
{
    for (int j = 0; j <= 3; ++j) {
        int t = (r >> j) & 7;
        if (t == 0 || t == 7) return false;
    }
    return true;
}

/* one‑time initialization of static tables */
static void init_tables(void)
{
    if (init_done) return;
    init_done = 1;

    /* find all valid rows */
    m = 0;
    for (int r = 0; r < 64; ++r) {
        if (good_row(r)) {
            valid_rows[m++] = (unsigned char)r;
        }
    }

    /* number of first rows (bit0 == 0) */
    m0 = 0;
    for (int i = 0; i < m; ++i) {
        if ((valid_rows[i] & 1) == 0) ++m0;
    }

    /* size of DP tables */
    dp_size = (size_t)m * (size_t)m;

    dp_cur = (seq_t *)calloc(dp_size, sizeof(seq_t));
    dp_next = (seq_t *)calloc(dp_size, sizeof(seq_t));

    /* precompute transitions */
    for (int i2 = 0; i2 < m; ++i2) {
        for (int i1 = 0; i1 < m; ++i1) {
            int state = i2 * m + i1;
            unsigned char *succ = trans_to[state];
            unsigned char len = 0;
            int a = valid_rows[i2];
            int b = valid_rows[i1];
            for (int i3 = 0; i3 < m; ++i3) {
                int c = valid_rows[i3];
                if (good_transition(a, b, c)) {
                    succ[len++] = (unsigned char)i3;
                }
            }
            trans_len[state] = len;
        }
    }

    /* initialize DP for n = 2: first row with bit0=0, second row arbitrary */
    for (int i2 = 0; i2 < m; ++i2) {
        if ((valid_rows[i2] & 1) == 0) {
            for (int i1 = 0; i1 < m; ++i1) {
                dp_cur[i2 * m + i1] = 1;
            }
        }
    }

    /* initialize cache: a(1) and a(2) */
    results_cap = 4;                     /* enough for indices 0..3 */
    results = (seq_t *)malloc(results_cap * sizeof(seq_t));
    results[1] = (seq_t)m0;              /* a(1) */
    results[2] = (seq_t)m0 * (seq_t)m;   /* a(2) */
    cache_max_n = 2;
}

/* compute next DP layer and store result */
static void extend_to(idx_t n_target)
{
    for (idx_t n = cache_max_n + 1; n <= n_target; ++n) {
        /* zero dp_next */
        for (size_t i = 0; i < dp_size; ++i) dp_next[i] = 0;

        /* transition */
        for (int s = 0; s < m * m; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int i1 = s % m;
            unsigned char *succ = trans_to[s];
            unsigned char len = trans_len[s];
            for (int t = 0; t < len; ++t) {
                int k = succ[t];
                dp_next[i1 * m + k] += val;
            }
        }

        /* compute sum of dp_next entries -> a(n) */
        seq_t sum = 0;
        for (size_t i = 0; i < dp_size; ++i) sum += dp_next[i];

        /* store result, ensure capacity */
        if (n >= results_cap) {
            size_t new_cap = results_cap * 2;
            if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
            results = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
            results_cap = new_cap;
        }
        results[n] = sum;

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        cache_max_n = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */
    init_tables();
    if (n <= cache_max_n) return results[n];
    extend_to(n);
    return results[n];
}
