/*
 * A207731  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding * the horizontal
 *            patterns 000 and 010 in every row, and * the vertical patterns
 *            011 and 101 in every column. * * The algorithm uses a
 *            dynamic‑program on the 9 admissible * 4‑bit column states. For
 *            each position n it stores the * number of arrays of leng...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* 9 admissible 4‑bit column patterns */
static const int state_values[9] = {
    0, 1, 2, 4, 8, 9, 12, 14, 15
};

/* Transition mask: trans_mask[i][j] has bit k set iff (i,j,k) is allowed. */
static uint16_t trans_mask[9][9];
static bool trans_inited = false;

static void init_transitions(void)
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            uint16_t mask = 0;
            int s_i = state_values[i];
            int s_j = state_values[j];
            for (int k = 0; k < 9; ++k) {
                int s_k = state_values[k];
                bool ok = true;
                /* Check each row of the 4×3 block (i,j,k) */
                for (int r = 0; r < 4 && ok; ++r) {
                    int b0 = (s_i >> r) & 1;
                    int b1 = (s_j >> r) & 1;
                    int b2 = (s_k >> r) & 1;
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 0 && b1 == 1 && b2 == 0))
                        ok = false;
                }
                if (ok) mask |= (1U << k);
            }
            trans_mask[i][j] = mask;
        }
    }
    trans_inited = true;
}

/* Cache for results a(1)..a(nmax) */
static seq_t *cache   = NULL;   /* memory that holds cached a(n) values */
static idx_t cache_cap = 0;     /* allocated size (number of entries) */
static idx_t nmax      = 0;     /* highest n with a(n) stored */

/* DP tables for length len: dp[i][j] = number of arrays ending with i,j */
static seq_t dp[9][9];
static seq_t dp_next[9][9];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* 1st and 2nd terms directly */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    /* Resize cache if necessary */
    if (n > cache_cap) {
#ifdef __cplusplus
        /* C++: use operator new[] for proper construction */
        seq_t *new_cache = new seq_t[n + 1];
        if (cache) {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
#else
        /* C: use realloc and cast to seq_t* (seq_t is a plain type) */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        /* old cache values are automatically preserved by realloc */
#endif
        cache = new_cache;
        cache_cap = n;
    }

    /* Return cached value if available */
    if (n <= nmax) return cache[n];

    /* Initialise transitions once */
    if (!trans_inited) init_transitions();

    /* If we have not yet started DP, initialise from scratch */
    if (nmax < 2) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp[i][j] = (seq_t)1;   /* all 81 pairs for length 2 */
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)81;
        nmax = 2;
    }

    /* Extend DP from current nmax+1 up to n */
    for (idx_t len = nmax + 1; len <= n; ++len) {
        /* zero next table */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp_next[i][j] = 0;

        /* transition step */
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t cur = dp[i][j];
                if (cur == 0) continue;
                uint16_t mask = trans_mask[i][j];
                for (int k = 0; k < 9; ++k) {
                    if (mask & (1U << k))
                        dp_next[j][k] += cur;
                }
            }
        }

        /* total for this len */
        seq_t total = 0;
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                total += dp_next[i][j];
        cache[len] = total;

        /* prepare for next iteration: dp = dp_next */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp[i][j] = dp_next[i][j];
    }

    nmax = n;
    return cache[n];
}
