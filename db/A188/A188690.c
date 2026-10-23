/*
 * A188690  Number of nX4 binary arrays without the pattern 0 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A188690 : Number of n×4 binary arrays without a 010 ± pattern
 *
 * Let good rows be 4‑bit patterns that do not contain the horizontal
 * 010 pattern.  A transition from the two previous rows (p2,p1) to a new
 * row r is allowed iff none of the forbidden patterns occurs vertically,
 * on the main diagonal, or on the anti‑diagonal.  The recurrence can
 * therefore be expressed as a linear cellular automaton on the 256
 * possible pairs (p2,p1).  The DP described below updates a 256‑entry
 * array of counts; after n rows the sum of all counts gives a(n).
 *
 * Parameters:
 *   idx_t   –  unsigned 64‑bit index type supplied by the build.
 *   seq_t   –  arbitrary‑precision unsigned integer type supplied by
 *              the build (uint8_t, uint16_t, …, unsigned __int128, …).
 *              All arithmetic is performed using seq_t; only indices
 *              and loop counters are idx_t.
 *
 * Complexity:
 *   O(n·256·16) ≈ 4096·n elementary operations, well below the
 *   required limit for n up to 10 000.
 */

#include <stdbool.h>

static bool good_row[16];
static bool allowed[16][16][16];
static unsigned int good_count = 0;

/* initialise good_row and allowed tables once */
static void init(void)
{
    for (int pat = 0; pat < 16; ++pat) {
        bool ok = true;
        /* horizontal 010 in the 4‑bit pattern */
        for (int j = 0; j <= 1; ++j) {
            if (((pat >> j) & 7u) == 0x2u) {
                ok = false;
                break;
            }
        }
        good_row[pat] = ok;
        if (ok) ++good_count;
    }

    for (int p2 = 0; p2 < 16; ++p2) {
        if (!good_row[p2]) continue;
        for (int p1 = 0; p1 < 16; ++p1) {
            if (!good_row[p1]) continue;
            for (int r = 0; r < 16; ++r) {
                if (!good_row[r]) continue;
                bool ok = true;

                /* vertical 0‑1‑0 */
                for (int col = 0; col < 4; ++col) {
                    if ((((p2 >> col) & 1u) == 0u) &&
                        ((((p1 >> col) & 1u) == 1u) &&
                         (((r   >> col) & 1u) == 0u))) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* main diagonal 0‑1‑0 (shift +1) */
                for (int j = 0; j <= 1; ++j) {
                    if ((((p2 >> j)     & 1u) == 0u) &&
                        ((((p1 >> (j+1)) & 1u) == 1u) &&
                         (((r   >> (j+2)) & 1u) == 0u))) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* anti‑diagonal 0‑1‑0 (shift –1) */
                for (int j = 1; j <= 2; ++j) {
                    if ((((p2 >> (j+1)) & 1u) == 0u) &&
                        ((((p1 >> j)     & 1u) == 1u) &&
                         (((r   >> (j-1)) & 1u) == 0u))) {
                        ok = false;
                        break;
                    }
                }
                allowed[p2][p1][r] = ok;
            }
        }
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init(); initialized = true; }

    if (n == 0) return (seq_t)0;            /* (not used, but safe) */
    if (n == 1) return (seq_t)good_count;   /* single row */

    /* For n == 2 we could rely on DP, but return good_count² directly */
    if (n == 2) return (seq_t)good_count *
                    (seq_t)good_count;

    /* DP over pairs (prev2,prev1) */
    seq_t dp[256] = {0};
    seq_t next[256] = {0};

    /* initialisation for the first two rows */
    for (int p2 = 0; p2 < 16; ++p2) {
        if (!good_row[p2]) continue;
        for (int p1 = 0; p1 < 16; ++p1) {
            if (!good_row[p1]) continue;
            dp[p2 * 16 + p1] = 1;
        }
    }

    /* iterate rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next */
        for (int i = 0; i < 256; ++i) next[i] = 0;

        for (int p2 = 0; p2 < 16; ++p2) {
            if (!good_row[p2]) continue;
            for (int p1 = 0; p1 < 16; ++p1) {
                if (!good_row[p1]) continue;
                seq_t cur = dp[p2 * 16 + p1];
                if (!cur) continue;
                for (int r = 0; r < 16; ++r) {
                    if (!good_row[r]) continue;
                    if (!allowed[p2][p1][r]) continue;
                    next[p1 * 16 + r] += cur;
                }
            }
        }
        /* advance dp */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* sum all state counts */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += dp[i];
    return total;
}
