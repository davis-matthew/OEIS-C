/*
 * A004487  Sprague-Grundy values for game of Wyt Queens.
 * https://oeis.org/A004487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = G(6,n), the Sprague-Grundy value of position (6,n) in
 *            Wythoff's game (Wyt Queens), G(x,y) = mex{ G(x-k,y), G(x,y-k),
 *            G(x-k,y-k) : k >= 1 }; computed by building rows 0..6 of the
 *            table
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int *g_rows[7];
static long long g_cap = 0;

static void build(long long N)
{
    int x;
    long long y;
    char *seen[7];
    long long lo[7];
    for (x = 0; x < 7; x++) {
        free(g_rows[x]);
        g_rows[x] = (int *)malloc((size_t)N * sizeof(int));
        seen[x] = (char *)calloc((size_t)(N + 64), 1);
        lo[x] = 0;
    }
    for (y = 0; y < N; y++) {
        for (x = 0; x < 7; x++) {
            int S[16];
            int ns = 0, k, i;
            long long v;
            for (k = 1; k <= x; k++) {
                S[ns++] = g_rows[x - k][y];
                if (y - k >= 0) S[ns++] = g_rows[x - k][y - k];
            }
            v = lo[x];
            for (;;) {
                int hit = 0;
                while (seen[x][v]) v++;
                for (i = 0; i < ns; i++)
                    if (S[i] == v) { hit = 1; break; }
                if (!hit) break;
                v++;
            }
            g_rows[x][y] = (int)v;
            seen[x][v] = 1;
            while (seen[x][lo[x]]) lo[x]++;
        }
    }
    for (x = 0; x < 7; x++) free(seen[x]);
    g_cap = N;
}

seq_t a(idx_t n)
{
    if ((long long)n >= g_cap) {
        long long N = 2 * g_cap;
        if (N < (long long)n + 1) N = (long long)n + 1;
        if (N < 1024) N = 1024;
        build(N);
    }
    return (seq_t)g_rows[6][n];
}
