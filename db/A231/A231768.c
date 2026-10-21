/*
 * A231768  Number of (4+1)X(n+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   valid_first, valid_last
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A231768 – number of 5×(n+1) 0/1 arrays where no element has
    a strict majority of its horizontal, diagonal, and antidiagonal
    neighbours equal to one.
    This implementation uses a transfer‑matrix dynamic programming over
    the columns of the array.  The algorithm has no closed‑form
    formula; the DP runs in O(n) time with a small constant. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_first.h"
#include "../utils/valid_last.h"

/* ----------------------------------------------------------------------
 * Helper functions – all defined at file scope so they are legal C/C++.
 * ---------------------------------------------------------------------- */

/* Validate that column M satisfies the neighbour conditions when its
   left neighbor is L and its right neighbor is R.  Only the neighbour
   count that actually exists for each row is considered.  */
static bool valid_middle(uint8_t L, uint8_t M, uint8_t R)
{
    (void)M; /* M is actually needed only to access bit positions */
    for (int r = 0; r < 5; ++r) {
        int ones = 0;

        /* Horizontal neighbours */
        if (L & (1U << r)) ++ones;
        if (R & (1U << r)) ++ones;

        /* Diagonal and anti‑diagonal neighbours */
        if (r > 0) {
            if (L & (1U << (r - 1))) ++ones;   /* up‑left  */
            if (R & (1U << (r - 1))) ++ones;   /* up‑right */
        }
        if (r < 4) {
            if (L & (1U << (r + 1))) ++ones;   /* down‑left  */
            if (R & (1U << (r + 1))) ++ones;   /* down‑right */
        }

        /* Total number of neighbours that exist */
        int total = 2;          /* left & right always */
        if (r > 0) total += 2;  /* up‑left & up‑right */
        if (r < 4) total += 2;  /* down‑left & down‑right */

        if ((ones << 1) > total) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------
 * Pre‑computed tables – built once on first call.
 * ---------------------------------------------------------------------- */
static bool tables_built = false;
static uint32_t valid_next_mask[32][32];   /* [L][M] -> mask of legal R values */
static bool valid_first_table[32][32];
static bool valid_last_table[32][32];

static void build_tables(void)
{
    for (int L = 0; L < 32; ++L) {
        for (int M = 0; M < 32; ++M) {
            unsigned mask = 0;
            for (int R = 0; R < 32; ++R) {
                if (valid_middle((uint8_t)L, (uint8_t)M, (uint8_t)R))
                    mask |= (1U << R);
            }
            valid_next_mask[L][M] = mask;
        }
    }
    for (int M = 0; M < 32; ++M)
        for (int R = 0; R < 32; ++R)
            valid_first_table[M][R] = valid_first((uint8_t)M, (uint8_t)R);
    for (int L = 0; L < 32; ++L)
        for (int M = 0; M < 32; ++M)
            valid_last_table[L][M] = valid_last((uint8_t)L, (uint8_t)M);
    tables_built = true;
}

/* ----------------------------------------------------------------------
 * DP arrays – static so they persist across calls but are re‑initialised
 * for each computation.
 * ---------------------------------------------------------------------- */
static seq_t dp[32][32];
static seq_t dp_next[32][32];

/* ----------------------------------------------------------------------
 * Main sequence function.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!tables_built)
        build_tables();

    /* Number of columns is n+1.  At least 2 columns for n>=1. */
    const idx_t N = n + 1U;

    /* Reset DP for current call. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* Initialise with first two columns. */
    for (int c0 = 0; c0 < 32; ++c0) {
        for (int c1 = 0; c1 < 32; ++c1) {
            if (valid_first_table[c0][c1])
                dp[c0][c1] = (seq_t)1;
        }
    }

    /* Process remaining columns (col index from 2 to N-1). */
    for (idx_t col = 2; col < N; ++col) {
        /* Clear dp_next. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int left = 0; left < 32; ++left) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[left][curr];
                if (ways == (seq_t)0) continue;
                unsigned mask = valid_next_mask[left][curr];
                while (mask) {
                    /* Find a set bit. */
                    int next = __builtin_ctz(mask);
                    mask &= mask - 1;
                    dp_next[curr][next] += ways;
                }
            }
        }

        /* Copy dp_next -> dp for next iteration. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over all valid final column states. */
    seq_t ans = (seq_t)0;
    for (int left = 0; left < 32; ++left)
        for (int curr = 0; curr < 32; ++curr)
            if (valid_last_table[left][curr])
                ans += dp[left][curr];

    return ans;
}
