/*
 * A199903  Number of -2..2 arrays x(0..n-1) of n elements with zero sum, and
 *          adjacent elements not equal modulo three (with -1 modulo 3 = 2).
 * https://oeis.org/A199903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Counts arrays of length n with elements in {-2,-1,0,1,2},
   total sum zero, and adjacent elements not congruent modulo 3. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Memoization storage for a(n) */
static seq_t *a_vals = NULL;
static idx_t  a_vals_len = 0;

/* DP storage for the previous length */
static seq_t *dp_prev = NULL;
static idx_t  dp_prev_len = 0;   /* length of DP (number of elements) */
static idx_t  dp_prev_size = 0;  /* 4*len+1 for DP width */

/* Ensure a_vals has room up to index n (inclusive) */
static void ensure_a_size(idx_t n) {
    if (a_vals_len > n) return;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!tmp) abort();
    if (a_vals) {
        memcpy(tmp, a_vals, a_vals_len * sizeof(seq_t));
        for (idx_t i = a_vals_len; i < new_len; ++i) tmp[i] = 0;
        free(a_vals);
    } else {
        for (size_t i = 0; i < new_len; ++i) tmp[i] = 0;
    }
    a_vals = tmp;
    a_vals_len = new_len;
}

/* Return the OEIS sequence value a(n) */
seq_t a(idx_t n)  /* n >= 1 */
{
    if (n == 0) return 0;                 /* defensive */

    /* Return memoized value if already computed */
    if (a_vals_len > n && a_vals[n] != 0) return a_vals[n];

    /* Initialise DP for length 1 if not yet done */
    if (dp_prev == NULL) {
        idx_t len = 1;
        idx_t size = 4 * len + 1;          /* 5 */
        dp_prev = (seq_t*)malloc(sizeof(seq_t) * size * 3);
        if (!dp_prev) abort();
        memset(dp_prev, 0, sizeof(seq_t) * size * 3);

        idx_t offset = 2;                  /* for sum 0 */
        for (int v = -2; v <= 2; ++v) {
            int r = ((v % 3) + 3) % 3;     /* residue modulo 3 */
            dp_prev[(size_t)r * size + (size_t)(v + offset)] = 1;
        }
        dp_prev_len  = 1;
        dp_prev_size = size;

        /* Compute a(1) */
        seq_t ans = 0;
        for (int r = 0; r < 3; ++r)
            ans += dp_prev[(size_t)r * size + (size_t)offset];

        ensure_a_size(1);
        a_vals[1] = ans;
    }

    /* Extend DP up to the requested n */
    while (dp_prev_len < n) {
        idx_t prev_len = dp_prev_len;
        idx_t cur_len  = prev_len + 1;

        idx_t size_prev = dp_prev_size;              /* 4*prev_len+1 */
        idx_t size_cur  = 4 * cur_len + 1;             /* 4*cur_len+1 */

        seq_t *dp_cur = (seq_t*)malloc(sizeof(seq_t) * size_cur * 3);
        if (!dp_cur) abort();
        memset(dp_cur, 0, sizeof(seq_t) * size_cur * 3);

        int offset_prev = 2 * (int)prev_len;
        int offset_cur  = 2 * (int)cur_len;

        /* Transition */
        for (idx_t s_prev_idx = 0; s_prev_idx < size_prev; ++s_prev_idx) {
            int s_prev = (int)s_prev_idx - offset_prev;
            for (int r = 0; r < 3; ++r) {
                seq_t cnt = dp_prev[(size_t)r * size_prev + s_prev_idx];
                if (cnt == 0) continue;
                for (int v = -2; v <= 2; ++v) {
                    int r_new = ((v % 3) + 3) % 3;
                    if (r_new == r) continue;      /* adjacency constraint */
                    int new_sum = s_prev + v;
                    size_t new_idx = (size_t)(new_sum + offset_cur);
                    dp_cur[(size_t)r_new * size_cur + new_idx] += cnt;
                }
            }
        }

        /* Compute a(cur_len) */
        seq_t ans_cur = 0;
        for (int r = 0; r < 3; ++r)
            ans_cur += dp_cur[(size_t)r * size_cur + (size_t)offset_cur];

        ensure_a_size(cur_len);
        a_vals[cur_len] = ans_cur;

        /* Swap in the new DP and free the old one */
        free(dp_prev);
        dp_prev = dp_cur;
        dp_prev_len  = cur_len;
        dp_prev_size = size_cur;
    }

    return a_vals[n];
}
