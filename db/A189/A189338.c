/*
 * A189338  Number of nX4 binary arrays without the pattern 1 0 0 1
 *          diagonally, vertically, antidiagonally or horizontally.
 * https://oeis.org/A189338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays avoiding the pattern 1 0 0 1
 *            in any direction (horizontal, vertical, main diagonal, or
 *            anti‑diagonal)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 'seq_t' and 'idx_t' are supplied by the build system. */

static const uint8_t valid_rows[15] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15
};

static uint16_t trans_state[4096][16];
static uint8_t  trans_len[4096];
static bool     trans_init = false;

/* Precompute, once, for every 3‑row state the set of admissible next rows. */
static void init_transitions(void)
{
    for (int s = 0; s < 4096; ++s) {
        uint8_t r1 = (s >> 8) & 0xF;
        uint8_t r2 = (s >> 4) & 0xF;
        uint8_t r3 =  s        & 0xF;

        if (r1 == 9 || r2 == 9 || r3 == 9) {      /* any row equal to 9 is forbidden */
            trans_len[s] = 0;
            continue;
        }

        uint8_t count = 0;
        for (int k = 0; k < 15; ++k) {
            uint8_t r4 = valid_rows[k];
            bool bad = false;

            /* Vertical pattern 1 0 0 1 in any column. */
            for (int c = 0; c < 4; ++c) {
                if (((r1 >> c) & 1) && !((r2 >> c) & 1) &&
                    !((r3 >> c) & 1) &&  ((r4 >> c) & 1)) {
                    bad = true;
                    break;
                }
            }
            if (bad) continue;

            /* Main diagonal 1 0 0 1. */
            if ((r1 & 1) && !((r2 >> 1) & 1) && !((r3 >> 2) & 1) && ((r4 >> 3) & 1))
                continue;

            /* Anti‑diagonal 1 0 0 1. */
            if (((r1 >> 3) & 1) && !((r2 >> 2) & 1) &&
                !((r3 >> 1) & 1) &&  (r4 & 1))
                continue;

            trans_state[s][count++] = (uint16_t)((r2 << 8) | (r3 << 4) | r4);
        }
        trans_len[s] = count;
    }
    trans_init = true;
}

seq_t a(idx_t n)
{
    if (!trans_init) init_transitions();

    /* n < 4: only the horizontal restriction applies.
       There are 15 admissible rows, so a(n)=15^n. */
    if (n < 4) {
        seq_t res = 1;
        for (idx_t i = 0; i < n; ++i) res *= 15;
        return res;
    }

    static seq_t dp_cur[4096];
    static seq_t dp_next[4096];

    /* initial states (first three rows).  Every admissible triple has count 1. */
    for (int i = 0; i < 4096; ++i) dp_cur[i] = 0;
    for (int r1 = 0; r1 < 16; ++r1) if (r1 != 9)
        for (int r2 = 0; r2 < 16; ++r2) if (r2 != 9)
            for (int r3 = 0; r3 < 16; ++r3) if (r3 != 9) {
                uint16_t idx = (r1 << 8) | (r2 << 4) | r3;
                dp_cur[idx] = 1;
            }

    /* iterate from row 4 up to row n */
    for (idx_t row = 4; row <= n; ++row) {
        for (int i = 0; i < 4096; ++i) dp_next[i] = 0;

        for (int s = 0; s < 4096; ++s) {
            seq_t cnt = dp_cur[s];
            if (!cnt) continue;

            uint8_t len = trans_len[s];
            for (uint8_t t = 0; t < len; ++t) {
                uint16_t ns = trans_state[s][t];
                dp_next[ns] += cnt;
            }
        }

        /* copy dp_next back into dp_cur for the next iteration */
        for (int i = 0; i < 4096; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int i = 0; i < 4096; ++i) total += dp_cur[i];
    return total;
}
