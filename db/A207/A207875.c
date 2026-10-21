/*
 * A207875  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A207875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 52, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207875: number of n×4 binary arrays avoiding
 *  horizontal 000 and 101, and vertical 010 and 111.
 *
 *  The code builds all admissible 4‑bit rows, then uses
 *  dynamic programming over the rows.  For each pair of
 *  consecutive rows (p,q) it precomputes a bitmask of rows r
 *  that may follow without creating a forbidden vertical
 *  triple.  A DP table dp[cur] indexed by (prev,row) counts
 *  the arrangements up to the current row.
 *
 *  Complexity:  O(n·256).  Works for n up to ~10 000 in <2 s.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*  seq_t and idx_t are supplied by the build environment.
 *  They may be any unsigned integer type or an arbitrary‑precision type.
 *  --------------------------------------------------------------
 */

seq_t a(idx_t n)
{
    /* One‑time initialization of admissible rows and transitions */
    static bool initialized = false;
    static bool valid_row[16];
    static uint16_t next_mask[256];     /* next_mask[p*16+q] = bitmask of rows that can follow p,q */
    static int valid_count = 0;

    if (!initialized) {
        /* admissible 4‑bit rows (no horizontal 000 or 101) */
        for (int r = 0; r < 16; ++r) {
            bool ok = true;
            for (int j = 0; j <= 1 && ok; ++j) {
                uint8_t w = (r >> j) & 7;   /* length‑3 window */
                if (w == 0 || w == 5)        /* 000 or 101 */
                    ok = false;
            }
            valid_row[r] = ok;
            if (ok)
                ++valid_count;
        }

        /* precompute allowable transitions (p,q) -> r */
        for (int p = 0; p < 16; ++p) {
            for (int q = 0; q < 16; ++q) {
                uint16_t mask = 0;
                if (!valid_row[p] || !valid_row[q]) {
                    next_mask[p * 16 + q] = mask;
                    continue;
                }
                for (int r = 0; r < 16; ++r) {
                    if (!valid_row[r])
                        continue;
                    bool bad = false;
                    for (int j = 0; j < 4; ++j) {
                        int b2 = (p >> j) & 1;    /* row i-2, column j */
                        int b1 = (q >> j) & 1;    /* row i-1, column j */
                        int b0 = (r >> j) & 1;    /* row i,   column j */
                        int tr = (b2 << 2) | (b1 << 1) | b0; /* vertical triple */
                        if (tr == 2 || tr == 7) { /* 010 or 111 */
                            bad = true;
                            break;
                        }
                    }
                    if (!bad)
                        mask |= (1u << r);
                }
                next_mask[p * 16 + q] = mask;
            }
        }

        initialized = true;
    }

    /* Handle trivial cases n < 1 */
    if (n == 0)
        return (seq_t)0;

    /* n == 1: only horizontal constraints apply */
    if (n == 1)
        return (seq_t)valid_count;

    /* n == 2: vertical constraints do not apply */
    if (n == 2)
        return (seq_t)valid_count * (seq_t)valid_count;

    /* DP tables indexed by (prev_row*16 + current_row) */
    static seq_t dp_cur[256];
    static seq_t dp_next[256];

    /* initialise dp_cur for the first two rows */
    for (int i = 0; i < 256; ++i)
        dp_cur[i] = 0;

    for (int p = 0; p < 16; ++p) {
        if (!valid_row[p])
            continue;
        for (int q = 0; q < 16; ++q) {
            if (!valid_row[q])
                continue;
            dp_cur[p * 16 + q] = 1;
        }
    }

    /* build successive rows */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 256; ++i)
            dp_next[i] = 0;

        for (int p = 0; p < 16; ++p) {
            if (!valid_row[p])
                continue;
            for (int q = 0; q < 16; ++q) {
                if (!valid_row[q])
                    continue;
                seq_t cur_val = dp_cur[p * 16 + q];
                if (cur_val == 0)
                    continue;
                uint16_t mask = next_mask[p * 16 + q];
                uint16_t m = mask;
                while (m) {
                    int r = __builtin_ctz(m);
                    m &= m - 1;
                    dp_next[q * 16 + r] += cur_val;
                }
            }
        }

        /* move to next step */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all arrangements ending with any last two rows */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += dp_cur[i];

    return total;
}
