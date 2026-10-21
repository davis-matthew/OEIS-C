/*
 * A229376  Number of nX4 0..2 arrays avoiding 11 horizontally, 22 vertically
 *          and 00 diagonally or antidiagonally.
 * https://oeis.org/A229376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A229376: Number of n×4 arrays over {0,1,2} with no horizontally adjacent 1s,
   no vertically adjacent 2s, and no diagonally adjacent 0s.
   Computed via dynamic programming over the 4‑column row states. */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static data: row states and transition matrix */
    static bool initialized = false;
    static int S;                      /* number of allowed row states */
    static unsigned char allowed[81][4];/* digits of each allowed state */
    static int state_index[81];       /* mapping from base‑3 number to state id (-1 if forbidden) */
    static unsigned char trans[81][81];/* transition matrix (1=allowed, 0=disallowed) */

    if (!initialized) {
        /* Build allowed row states: no horizontally adjacent 1s */
        S = 0;
        for (int val = 0; val < 81; ++val) {    /* 3^4 possible rows */
            int tmp = val;
            unsigned char digits[4];
            for (int col = 0; col < 4; ++col) {
                digits[col] = tmp % 3;          /* least‑significant digit first */
                tmp /= 3;
            }
            bool ok = true;
            for (int col = 0; col < 3 && ok; ++col)
                if (digits[col] == 1 && digits[col + 1] == 1)
                    ok = false;
            if (ok) {
                for (int col = 0; col < 4; ++col)
                    allowed[S][col] = digits[col];
                state_index[val] = S;
                ++S;
            } else {
                state_index[val] = -1;
            }
        }

        /* Build transition matrix between allowed states */
        for (int i = 0; i < S; ++i)
            for (int j = 0; j < S; ++j) {
                bool ok = true;
                /* vertical 22 */
                for (int col = 0; col < 4 && ok; ++col)
                    if (allowed[i][col] == 2 && allowed[j][col] == 2)
                        ok = false;
                /* main diagonal 00 */
                for (int col = 0; col < 3 && ok; ++col)
                    if (allowed[i][col] == 0 && allowed[j][col + 1] == 0)
                        ok = false;
                /* anti‑diagonal 00 */
                for (int col = 0; col < 3 && ok; ++col)
                    if (allowed[i][col + 1] == 0 && allowed[j][col] == 0)
                        ok = false;
                trans[i][j] = ok ? 1 : 0;
            }

        initialized = true;
    }

    /* Dynamic programming across the rows */
    seq_t prev[81], next[81];

    /* first row: each allowed state occurs exactly once */
    for (int i = 0; i < S; ++i) prev[i] = 1;

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next array */
        for (int j = 0; j < S; ++j) next[j] = 0;
        /* compute transitions */
        for (int i = 0; i < S; ++i) {
            if (prev[i] == 0) continue;
            for (int j = 0; j < S; ++j) {
                if (trans[i][j])
                    next[j] += prev[i];
            }
        }
        /* copy next to prev for the next iteration */
        for (int j = 0; j < S; ++j) prev[j] = next[j];
    }

    /* sum over all ending states */
    seq_t result = 0;
    for (int i = 0; i < S; ++i) result += prev[i];

    return result;
}
