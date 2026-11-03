/*
 * A206997  Number of nX5 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×5 binary arrays avoiding horizontal 010 and vertical
 *            011. Computation by dynamic programming over the last two rows
 *            (state size 21²). Initial values: a(1)=21, a(2)=441
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static void init_state(unsigned char *valid_rows, int *valid_index,
                       bool (*trans)[21][21], size_t *num_valid) {
    /* Build list of 5‑bit rows that avoid horizontal pattern 010 */
    for (int p = 0; p < 32; ++p) valid_index[p] = -1;
    *num_valid = 0;
    for (int pat = 0; pat < 32; ++pat) {
        bool bad = false;
        for (int j = 0; j <= 2; ++j)
            if (((pat >> j) & 7) == 0b010) { bad = true; break; }
        if (!bad) {
            valid_index[pat] = (int)*num_valid;
            valid_rows[*num_valid] = (unsigned char)pat;
            (*num_valid)++;
        }
    }
    /* Pre‑compute transitions for triples (prev2, prev1) → curr */
    for (size_t i = 0; i < *num_valid; ++i)
        for (size_t j = 0; j < *num_valid; ++j)
            for (size_t k = 0; k < *num_valid; ++k) {
                bool ok = true;
                for (int bit = 0; bit < 5; ++bit)
                    if (((valid_rows[i] >> bit) & 1) == 0 &&
                        ((valid_rows[j] >> bit) & 1) == 1 &&
                        ((valid_rows[k] >> bit) & 1) == 1) {
                        ok = false;
                        break;
                    }
                trans[i][j][k] = ok;
            }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 21;
    if (n == 2) return 441;

    /* State information (static, initialized once) */
    static unsigned char valid_rows[21];
    static int valid_index[32];
    static bool trans[21][21][21];
    static bool initialized = false;
    static size_t num_valid = 0;

    if (!initialized) {
        init_state(valid_rows, valid_index, trans, &num_valid);
        initialized = true;
    }

    /* DP arrays */
    static seq_t dp_cur[21][21];
    static seq_t dp_next[21][21];

    /* initialise for first two rows: each pair can occur once */
    for (size_t i = 0; i < num_valid; ++i)
        for (size_t j = 0; j < num_valid; ++j)
            dp_cur[i][j] = 1;

    /* iterate rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        for (size_t i = 0; i < num_valid; ++i)
            for (size_t j = 0; j < num_valid; ++j)
                dp_next[i][j] = 0;
        for (size_t i = 0; i < num_valid; ++i) {
            for (size_t j = 0; j < num_valid; ++j) {
                seq_t cnt = dp_cur[i][j];
                if (!cnt) continue;
                for (size_t k = 0; k < num_valid; ++k)
                    if (trans[i][j][k])
                        dp_next[j][k] += cnt;
            }
        }
        /* copy next to current for next iteration */
        for (size_t i = 0; i < num_valid; ++i)
            for (size_t j = 0; j < num_valid; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* sum over all states to get total number of arrays */
    seq_t total = 0;
    for (size_t i = 0; i < num_valid; ++i)
        for (size_t j = 0; j < num_valid; ++j)
            total += dp_cur[i][j];
    return total;
}
