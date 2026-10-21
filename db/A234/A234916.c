/*
 * A234916  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A234916 – Number of (n+1)×4 arrays with 0..4 entries, no equal
    neighbours and each 2×2 block having diagonal and antidiagonal
    sums differing by 2 (absolute difference 2).
    The sequence is computed by a transfer‑matrix DP:
    * All 320 possible rows of length 4 (respecting horizontal distinctness)
      are enumerated.
    * Compatibility between two consecutive rows is pre‑computed:
          vertical neighbours are distinct;
          for each adjacent pair of columns c and c+1
            |(a[c] + b[c+1]) – (a[c+1] + b[c])| == 2
    * The number of arrays of height L = n+1 equals
          1ᵀ · M^(L‑1) · 1
      where M is the compatibility matrix.
      We evaluate this by iterating L‑1 times over the sparse
      adjacency lists. */

#include <stdbool.h>
#include <stdint.h>

#define ROWS 320
static unsigned char row_vals[ROWS][4];
static uint16_t compat[ROWS][ROWS];
static uint16_t compat_cnt[ROWS];
static bool init_done = false;

static void init_rows_and_compat(void)
{
    if (init_done) return;
    int idx = 0;
    /* generate all valid rows: 4 digits, each 0..4, adjacent distinct */
    for (int a = 0; a <= 4; a++)
        for (int b = 0; b <= 4; b++)
            if (b != a)
                for (int c = 0; c <= 4; c++)
                    if (c != b)
                        for (int d = 0; d <= 4; d++)
                            if (d != c)
                                {
                                    row_vals[idx][0] = a;
                                    row_vals[idx][1] = b;
                                    row_vals[idx][2] = c;
                                    row_vals[idx][3] = d;
                                    idx++;
                                }
    /* build compatibility lists */
    for (int i = 0; i < ROWS; i++)
        {
            compat_cnt[i] = 0;
            for (int j = 0; j < ROWS; j++)
                {
                    /* vertical adjacency */
                    bool ok = true;
                    for (int k = 0; k < 4; k++)
                        if (row_vals[i][k] == row_vals[j][k])
                            { ok = false; break; }
                    if (!ok) continue;
                    /* 2×2 block condition for columns 0..2 */
                    for (int k = 0; k < 3; k++)
                        {
                            int diff = (int)row_vals[i][k]
                                      + (int)row_vals[j][k+1]
                                      - (int)row_vals[i][k+1]
                                      - (int)row_vals[j][k];
                            if (diff != 2 && diff != -2)
                                { ok = false; break; }
                        }
                    if (!ok) continue;
                    compat[i][compat_cnt[i]++] = (uint16_t)j;
                }
        }
    init_done = true;
}

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range indices (beyond offset). */
    if (n < 1) return (seq_t)0;
    init_rows_and_compat();
    const idx_t steps = n;               /* L-1 transitions for L = n+1 rows */
    seq_t cur[ROWS];
    seq_t nxt[ROWS];
    seq_t zero = (seq_t)0;
    /* initialize cur */
    for (int i = 0; i < ROWS; i++) cur[i] = (seq_t)1;
    for (idx_t step = 0; step < steps; ++step)
        {
            /* zero nxt */
            for (int i = 0; i < ROWS; i++) nxt[i] = zero;
            /* propagate */
            for (int i = 0; i < ROWS; i++)
                {
                    seq_t vi = cur[i];
                    if (vi == zero) continue;
                    uint16_t lim = compat_cnt[i];
                    for (uint16_t t = 0; t < lim; ++t)
                        {
                            uint16_t j = compat[i][t];
                            nxt[j] += vi;
                        }
                }
            /* copy nxt back to cur */
            for (int i = 0; i < ROWS; i++) cur[i] = nxt[i];
        }
    /* sum all ending rows */
    seq_t total = zero;
    for (int i = 0; i < ROWS; i++) total += cur[i];
    return total;
}
