/*
 * A207930  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A207930
   Number of 4×n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
   and 0 0 0 and 1 1 1 vertically.
   Computes by dynamic programming over columns.
   Uses the recurrence:
   a(1)=10, a(2)=100,
   for n≥3: a(n)=∑_{c,d,e∈C} a_{n-1}(c,d) with 4‑column transitions
   satisfying the horizontal/vertical constraints.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* width 4, columns are 4-bit vectors */
    int allowed[16], m = 0;
    int bits[10][4];

    /* find all columns satisfying the vertical constraints */
    for (int col = 0; col < 16; ++col)
    {
        int ok = 1;
        for (int r = 0; r < 2; ++r)
        {
            int b0 = (col >> r) & 1;
            int b1 = (col >> (r + 1)) & 1;
            int b2 = (col >> (r + 2)) & 1;
            if (b0 == b1 && b1 == b2) { ok = 0; break; }
        }
        if (ok) { allowed[m++] = col; }
    }

    /* store bits of each allowed column for fast access */
    for (int i = 0; i < m; ++i)
    {
        int col = allowed[i];
        for (int r = 0; r < 4; ++r)
            bits[i][r] = (col >> r) & 1;
    }

    /* pre‑compute horizontal‑transition validity */
    size_t states = (size_t)m * m;
    uint8_t *valid = (uint8_t *)malloc(states * m);
    if (!valid) return 0; /* allocation failure, unlikely */

    for (int u = 0; u < m; ++u)
    {
        for (int v = 0; v < m; ++v)
        {
            for (int w = 0; w < m; ++w)
            {
                int good = 1;
                for (int r = 0; r < 4; ++r)
                {
                    int a = bits[u][r];
                    int b = bits[v][r];
                    int c = bits[w][r];
                    if (a == 0 && b == 0 && c == 0) { good = 0; break; }
                    if (a == 0 && b == 1 && c == 0) { good = 0; break; }
                }
                size_t idx = ((size_t)u * m + v) * m + w;
                valid[idx] = (uint8_t)good;
            }
        }
    }

    /* handle small n separately */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)m;
    if (n == 2) return (seq_t)m * (seq_t)m;

    /* dynamic programming */
    seq_t *dp_prev = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(states, sizeof(seq_t));
    if (!dp_prev || !dp_next) { free(valid); return 0; }

    /* for column 2: all pairs are allowed */
    for (size_t i = 0; i < states; ++i) dp_prev[i] = 1;

    int N = (int)n;
    for (int step = 3; step <= N; ++step)
    {
        /* zero next array */
        memset(dp_next, 0, states * sizeof(seq_t));

        for (int u = 0; u < m; ++u)
        {
            for (int v = 0; v < m; ++v)
            {
                size_t idx = (size_t)u * m + v;
                seq_t val = dp_prev[idx];
                if (!val) continue;

                for (int w = 0; w < m; ++w)
                {
                    if (!valid[((size_t)u * m + v) * m + w]) continue;
                    size_t next_idx = (size_t)v * m + w;
                    dp_next[next_idx] += val;
                }
            }
        }

        /* swap dp_prev and dp_next */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* sum all states for final answer */
    seq_t total = 0;
    for (size_t i = 0; i < states; ++i) total += dp_prev[i];

    free(valid);
    free(dp_prev);
    free(dp_next);

    return total;
}
