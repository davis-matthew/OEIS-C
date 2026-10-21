/*
 * A207244  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays that avoid horizontal 000 and
 *            011 and vertical 001 and 010. Computed by transfer-matrix DP on
 *            9 admissible column patterns
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 61, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 9 allowed column patterns (vertical constraints only). */
    static const uint8_t column_vals[9] =
        {0, 6, 7, 8, 11, 12, 13, 14, 15};           /* 4‑bit columns */

    /* Transition table: trans[i][j] contains a bitmask whose k‑th bit
       is set iff placing column k after columns i and j does not
       create the forbidden horizontal triples in any row. */
    static uint16_t trans[9][9];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                uint16_t mask = 0;
                for (int k = 0; k < 9; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        unsigned int a = (column_vals[i] >> r) & 1;
                        unsigned int b = (column_vals[j] >> r) & 1;
                        unsigned int c = (column_vals[k] >> r) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) mask |= 1U << k;
                }
                trans[i][j] = mask;
            }
        }
        init = true;
    }

    /* Handle the first two columns directly. */
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    if (n == 1) return (seq_t)9;          /* 9 admissible single columns */
    if (n == 2) return (seq_t)81;         /* 9 × 9 complete pairs */

    /* DP arrays: ways[i][j] = number of ways ending with columns (i,j). */
    seq_t ways[9][9];
    seq_t next[9][9];

    /* Length 2: every pair of admissible columns is allowed. */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            ways[i][j] = (seq_t)1;

    /* Iteratively extend to length n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero the next array using element assignment (not memcpy). */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                next[i][j] = (seq_t)0;

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t val = ways[i][j];
                if (!val) continue;
                uint16_t mask = trans[i][j];
                while (mask) {
                    uint16_t bit = mask & -(int)mask;
                    int k = __builtin_ctz((unsigned int)bit);
                    next[j][k] += val;
                    mask ^= bit;
                }
            }
        }

        /* Copy next into ways using element assignment. */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                ways[i][j] = next[i][j];
    }

    /* Sum all ways for length n. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            total += ways[i][j];

    return total;
}
