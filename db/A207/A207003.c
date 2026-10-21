/*
 * A207003  Number of 6Xn 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A207003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays with no horizontal 0 1 0 and
 *            no vertical 0 1 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   has_vertical_011
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are computed by a transfer matrix over the 33 column states that avoid 011 vertically. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_vertical_011.h"

seq_t a(idx_t n)
{
    /* 1. Collect all 6‑bit column states that avoid a vertical 0 1 1. */
    unsigned char S[64];
    int Ssize = 0;
    int idx_of[64];
    for (int i = 0; i < 64; ++i) idx_of[i] = -1;
    for (int val = 0; val < 64; ++val)
    {
        if (!has_vertical_011((unsigned char)val))
        {
            idx_of[val] = Ssize;
            S[Ssize++] = (unsigned char)val;
        }
    }

    /* 2. Pre‑compute allowed transitions.
       For each pair (u,v) of states we store all w that satisfy the horizontal
       0 1 0 rule. */
    const int MAX_NEIGH = 33;   /* max number of w per pair (exactly |S| ≤ 33) */
    struct Trans { unsigned char count; unsigned char next[MAX_NEIGH]; };
    int total_pairs = Ssize * Ssize;
    struct Trans *trans = (struct Trans *)malloc((size_t)total_pairs * sizeof(struct Trans));
    for (int u_idx = 0; u_idx < Ssize; ++u_idx)
    {
        unsigned char u = S[u_idx];
        for (int v_idx = 0; v_idx < Ssize; ++v_idx)
        {
            unsigned char v = S[v_idx];
            int pair_idx = u_idx * Ssize + v_idx;
            struct Trans *t = &trans[pair_idx];
            t->count = 0;
            for (int w_idx = 0; w_idx < Ssize; ++w_idx)
            {
                unsigned char w = S[w_idx];
                bool ok = true;
                for (int r = 0; r < 6; ++r)
                {
                    int a = (u >> r) & 1;
                    int b = (v >> r) & 1;
                    int c = (w >> r) & 1;
                    if (a == 0 && b == 1 && c == 0)
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                {
                    t->next[t->count++] = (unsigned char)w_idx;
                }
            }
        }
    }

    /* 3. Handle the trivial cases (n = 1, 2). */
    if (n == 1)
    {
        free(trans);
        return (seq_t)Ssize;
    }
    if (n == 2)
    {
        free(trans);
        return (seq_t)Ssize * (seq_t)Ssize;
    }

    /* 4. Dynamic programming for n ≥ 3. */
    seq_t *dp      = (seq_t *)calloc((size_t)total_pairs, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc((size_t)total_pairs, sizeof(seq_t));

    /* Initial DP: every pair of columns is allowed once. */
    for (int idx = 0; idx < total_pairs; ++idx)
        dp[idx] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step)
    {
        memset(dp_next, 0, (size_t)total_pairs * sizeof(seq_t));
        for (int idx = 0; idx < total_pairs; ++idx)
        {
            seq_t val = dp[idx];
            if (val == (seq_t)0) continue;
            struct Trans *t = &trans[idx];
            for (unsigned char k = 0; k < t->count; ++k)
            {
                int next_idx = ((idx % Ssize) * Ssize) + t->next[k];
                dp_next[next_idx] += val;
            }
        }
        /* swap dp and dp_next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* 5. Sum all entries of the final DP array. */
    seq_t result = (seq_t)0;
    for (int idx = 0; idx < total_pairs; ++idx)
        result += dp[idx];

    /* Clean up. */
    free(dp);
    free(dp_next);
    free(trans);

    return result;
}
