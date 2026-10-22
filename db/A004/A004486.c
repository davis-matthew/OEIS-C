/*
 * A004486  Sprague-Grundy values for game of Wyt Queens.
 * https://oeis.org/A004486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = G(5,n), the Sprague-Grundy value of position (5,n) in
 *            Wythoff's game (Wyt Queens), computed by a mex search over rows
 *            0..5
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define WROWS 6

static int *G[WROWS];
static unsigned char *seen[WROWS];
static long long cap = 0;

static void build(long long N)
{
    int x;
    long long y, rowmex[WROWS];
    for (x = 0; x < WROWS; x++) {
        free(G[x]);
        free(seen[x]);
        G[x] = (int *)malloc((size_t)(N + 1) * sizeof(int));
        seen[x] = (unsigned char *)calloc((size_t)(N + 2 * WROWS + 16), 1);
        rowmex[x] = 0;
    }
    for (y = 0; y <= N; y++) {
        for (x = 0; x < WROWS; x++) {
            int e[2 * WROWS];
            int k = 0, i, j;
            long long v;
            for (i = 1; i <= x; i++)
                e[k++] = G[x - i][y];
            for (i = 1; i <= x && i <= y; i++)
                e[k++] = G[x - i][y - i];
            v = rowmex[x];
            for (;;) {
                bool hit = false;
                while (seen[x][v]) v++;
                for (j = 0; j < k; j++)
                    if ((long long)e[j] == v) { hit = true; break; }
                if (!hit) break;
                v++;
            }
            G[x][y] = (int)v;
            seen[x][v] = 1;
            while (seen[x][rowmex[x]]) rowmex[x]++;
        }
    }
    cap = N + 1;
}

seq_t a(idx_t n)
{
    if ((long long)n >= cap) {
        long long N = (long long)n;
        if (N < 2 * cap) N = 2 * cap;
        if (N < 64) N = 64;
        build(N);
    }
    return (seq_t)G[5][n];
}
