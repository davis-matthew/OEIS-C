/*
 * A206886  Number of nX5 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays avoiding the horizontal pattern
 *            0 1 0 and the vertical pattern 1 0 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* 21 admissible 5‑bit rows (no horizontal 010). */
static const uint8_t rows[21] = {
    0, 1, 3, 6, 7, 12, 13, 14, 15, 16, 17, 19, 22, 23, 24, 25,
    27, 28, 29, 30, 31
};

/* 21×21 transition masks: trans[a][b] is a bitmask of rows c that can
   follow rows a,b without creating the forbidden vertical pattern. */
static uint32_t trans[21][21];
static int trans_built = 0;

static void build_transitions(void) {
    if (trans_built) return;
    for (int a = 0; a < 21; ++a) {
        for (int b = 0; b < 21; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 21; ++c) {
                int ok = 1;
                for (int j = 0; j < 5 && ok; ++j) {
                    int va = (rows[a] >> j) & 1;
                    int vb = (rows[b] >> j) & 1;
                    int vc = (rows[c] >> j) & 1;
                    if (va && !vb && !vc) ok = 0;
                }
                if (ok) mask |= (1U << c);
            }
            trans[a][b] = mask;
        }
    }
    trans_built = 1;
}

seq_t a(idx_t n)
{
    build_transitions();

    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)441;   /* 21² */

    /* Local DP tables; size small enough for stack. */
    seq_t dp_prev[21][21];
    seq_t dp_cur[21][21];

    /* Initialise dp_prev for pair of rows (length 2). */
    for (int a = 0; a < 21; ++a)
        for (int b = 0; b < 21; ++b)
            dp_prev[a][b] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_cur */
        for (int a = 0; a < 21; ++a)
            for (int b = 0; b < 21; ++b)
                dp_cur[a][b] = (seq_t)0;

        for (int a = 0; a < 21; ++a) {
            for (int b = 0; b < 21; ++b) {
                seq_t val = dp_prev[a][b];
                if (!val) continue;
                uint32_t mask = trans[a][b];
                /* iterate over set bits in mask */
                for (int bit = 0; bit < 21; ++bit) {
                    if (mask & (1U << bit))
                        dp_cur[b][bit] += val;
                }
            }
        }
        /* copy dp_cur to dp_prev for next iteration */
        for (int a = 0; a < 21; ++a)
            for (int b = 0; b < 21; ++b)
                dp_prev[a][b] = dp_cur[a][b];
    }

    /* Sum all states for the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 21; ++a)
        for (int b = 0; b < 21; ++b)
            total += dp_prev[a][b];

    return total;
}
