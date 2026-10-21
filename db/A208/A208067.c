/*
 * A208067  Number of n X 6 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 135, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208067
 * a(n) = number of n×6 binary arrays avoiding 000/111 horizontally and 001/011 vertically.
 * The values are computed by dynamic programming over row states.
 * Initial horizontal‑allowed rows (no 000 or 111) and vertical transition masks are precomputed once.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of seq_t (provided by the build) */
typedef seq_t seq_t;
typedef idx_t idx_t;

static bool init_done = false;          /* indicates if precomputation finished */
static bool horizontal_allowed[64];     /* whether a row pattern is horizontal‑allowed */
static uint64_t transition_mask[4096];  /* for each pair (a,b) bitmask of allowed next rows c */
static int H_global = 0;                /* number of horizontal‑allowed rows */

/* Precompute horizontal constraints and vertical transition masks */
static void precompute(void)
{
    if (init_done)
        return;

    /* Count horizontal‑allowed rows */
    int count = 0;
    for (int r = 0; r < 64; ++r) {
        bool ok = true;
        /* check every block of three consecutive columns */
        for (int j = 0; j < 4; ++j) {
            int bits = (r >> j) & 7;   /* 3‑bit window */
            if (bits == 0 || bits == 7) { ok = false; break; }
        }
        horizontal_allowed[r] = ok;
        if (ok) ++count;
    }
    H_global = count;

    /* Precompute transition masks for each pair (a,b) */
    for (int a = 0; a < 64; ++a) {
        if (!horizontal_allowed[a])
            continue;
        for (int b = 0; b < 64; ++b) {
            if (!horizontal_allowed[b])
                continue;
            uint64_t mask = 0;
            for (int c = 0; c < 64; ++c) {
                if (!horizontal_allowed[c])
                    continue;
                bool ok = true;
                /* check vertical 3‑row patterns in all 6 columns */
                for (int j = 0; j < 6; ++j) {
                    int f0 = (a >> j) & 1;
                    int f1 = (b >> j) & 1;
                    int f2 = (c >> j) & 1;
                    if (f0 == 0 && f1 == 0 && f2 == 1) { ok = false; break; }
                    if (f0 == 0 && f1 == 1 && f2 == 1) { ok = false; break; }
                }
                if (ok)
                    mask |= (1ULL << c);
            }
            transition_mask[a * 64 + b] = mask;
        }
    }

    init_done = true;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* out of range, but undefined behavior is fine */

    precompute();

    /* Special cases for small n */
    if (n == 1)
        return (seq_t)H_global;
    if (n == 2)
        return (seq_t)H_global * H_global;

    /* DP tables over pairs of rows */
    static seq_t dp[4096];
    static seq_t dp_next[4096];
    /* initialize dp for the first two rows (all allowed pairs) */
    for (int i = 0; i < 4096; ++i)
        dp[i] = 0;
    for (int a = 0; a < 64; ++a) {
        if (!horizontal_allowed[a])
            continue;
        for (int b = 0; b < 64; ++b) {
            if (!horizontal_allowed[b])
                continue;
            dp[a * 64 + b] = 1;
        }
    }

    /* Iterate rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 4096; ++i)
            dp_next[i] = 0;

        for (int a = 0; a < 64; ++a) {
            if (!horizontal_allowed[a])
                continue;
            for (int b = 0; b < 64; ++b) {
                if (!horizontal_allowed[b])
                    continue;
                seq_t val = dp[a * 64 + b];
                if (val == 0)
                    continue;
                uint64_t mask = transition_mask[a * 64 + b];
                while (mask) {
                    uint64_t lsb = mask & -mask;
                    int c = __builtin_ctzll(mask);
                    dp_next[b * 64 + c] += val;
                    mask ^= lsb;
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < 4096; ++i)
            dp[i] = dp_next[i];
    }

    /* Sum over all final pairs */
    seq_t total = 0;
    for (int i = 0; i < 4096; ++i)
        total += dp[i];

    return total;
}
