/*
 * A240595  Look-and-Say table, where in row(n+1) the sorted list of distinct
 *          terms of row(n) is preceded by the list of numbers of their
 *          occurrences.
 * https://oeis.org/A240595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: generates the Look‑and‑Say table as defined in OEIS A240595. */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* --- Initialization (executed once) --- */
    static bool initialized = false;
    /* rows[1..13] are stored; rowlen[1..13] holds lengths; cumulative[0..13] holds cumulative term counts. */
    static seq_t rows[14][8];      /* rows[0] unused; max row length is 8 for A240595 */
    static idx_t rowlen[14];
    static idx_t cumulative[14];

    if (!initialized) {
        /* row 1 */
        rows[1][0] = (seq_t)1;
        rowlen[1] = 1;
        cumulative[0] = 0;
        cumulative[1] = 1;

        idx_t cur = 1; /* index of the last generated row */

        while (true) {
            /* ---- Build the next row into tempRow ---- */
            idx_t prevLen = rowlen[cur];
            seq_t temp[64];   /* temporary buffer for the previous row */
            for (idx_t i = 0; i < prevLen; ++i) temp[i] = rows[cur][i];

            /* sort temp ascending (insertion sort – row lengths are tiny) */
            for (idx_t i = 1; i < prevLen; ++i) {
                seq_t key = temp[i];
                idx_t j = i;
                while (j > 0 && temp[j - 1] > key) {
                    temp[j] = temp[j - 1];
                    --j;
                }
                temp[j] = key;
            }

            /* gather counts and distinct values */
            seq_t counts[64];
            seq_t values[64];
            idx_t k = 0; /* number of distinct values */
            idx_t i = 0;
            while (i < prevLen) {
                seq_t val = temp[i];
                idx_t cnt = 0;
                while (i < prevLen && temp[i] == val) {
                    ++cnt;
                    ++i;
                }
                counts[k] = cnt;
                values[k] = val;
                ++k;
            }

            /* construct the new row: counts followed by values */
            idx_t newLen = 2 * k;
            seq_t newRow[64];
            for (idx_t j = 0; j < k; ++j) newRow[j] = counts[j];
            for (idx_t j = 0; j < k; ++j) newRow[k + j] = values[j];

            /* ---- Check for fixed point ---- */
            bool equal = (newLen == rowlen[cur]);
            if (equal) {
                for (idx_t j = 0; j < newLen; ++j) {
                    if (newRow[j] != rows[cur][j]) {
                        equal = false;
                        break;
                    }
                }
            }
            if (equal) break;  /* current row is stable; stop here */

            /* ---- Store the new row ---- */
            ++cur; /* next row index */
            rowlen[cur] = newLen;
            for (idx_t j = 0; j < newLen; ++j) rows[cur][j] = newRow[j];
            cumulative[cur] = cumulative[cur - 1] + newLen;
        }
        initialized = true;
    }

    /* --- Compute a(n) by locating the corresponding row --- */
    if (n <= cumulative[12]) {
        idx_t r = 1;
        while (cumulative[r] < n) ++r;           /* find row r containing index n */
        idx_t offset = n - cumulative[r - 1] - 1; /* zero‑based offset within row r */
        return rows[r][offset];
    } else {
        /* After row 13 the table repeats the same row. */
        idx_t pos = (n - cumulative[12] - 1) % rowlen[13];
        return rows[13][pos];
    }
}
