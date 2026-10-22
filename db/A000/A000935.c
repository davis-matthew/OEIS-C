/*
 * A000935  Number of free planar polyenoids with 2n nodes and symmetry point
 *          group C_{2h}.
 * https://oeis.org/A000935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   exhaustive search. A C_{2h} polyenoid with 2n nodes has a
 *            central bond AB with 180-degree rotational symmetry. Count T =
 *            embeddings of the n-node half tree rooted at A, placed on the
 *            hexagonal lattice together with its rotated copy, with no two
 *            nodes on the same site. Count M = those embeddings t...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int g_W, g_OFF, g_mode;
static unsigned char *g_occ;
static uint64_t g_cnt;
static int *g_SX, *g_SY, *g_PX, *g_PY;

static int cell(int x, int y) { return (x + g_OFF) * g_W + (y + g_OFF); }

static void rec(int top, int rem)
{
    int x, y, px, py, k, i, j, c, ix[4], iy[4], nx[3], ny[3];
    if (rem == 0) { g_cnt++; return; }
    if (top == 0) return;
    top--;
    x = g_SX[top]; y = g_SY[top]; px = g_PX[top]; py = g_PY[top];
    /* leave this slot empty */
    rec(top, rem);
    /* fill this slot */
    k = g_mode ? 4 : 2;
    ix[0] = x;  iy[0] = y;
    ix[1] = -x; iy[1] = 1 - y;
    ix[2] = -x; iy[2] = y;
    ix[3] = x;  iy[3] = 1 - y;
    for (i = 0; i < k; i++) {
        if (g_occ[cell(ix[i], iy[i])]) goto restore;
        for (j = 0; j < i; j++)
            if (ix[i] == ix[j] && iy[i] == iy[j]) goto restore;
    }
    for (i = 0; i < k; i++) g_occ[cell(ix[i], iy[i])] = 1;
    nx[0] = x - 1; ny[0] = y;
    nx[1] = x + 1; ny[1] = y;
    nx[2] = x;     ny[2] = (((x + y) % 2) == 0) ? y + 1 : y - 1;
    c = top;
    for (i = 0; i < 3; i++) {
        if (nx[i] == px && ny[i] == py) continue;
        g_SX[c] = nx[i]; g_SY[c] = ny[i]; g_PX[c] = x; g_PY[c] = y;
        c++;
    }
    rec(c, rem - 1);
    for (i = 0; i < k; i++) g_occ[cell(ix[i], iy[i])] = 0;
restore:
    g_SX[top] = x; g_SY[top] = y; g_PX[top] = px; g_PY[top] = py;
}

static uint64_t run(idx_t n, int mode)
{
    long long sz;
    int cap = (int)n + 8;
    g_OFF = (int)n + 3;
    g_W = 2 * (int)n + 7;
    sz = (long long)g_W * g_W;
    g_occ = (unsigned char *)calloc((size_t)sz, 1);
    g_SX = (int *)malloc(sizeof(int) * (size_t)cap);
    g_SY = (int *)malloc(sizeof(int) * (size_t)cap);
    g_PX = (int *)malloc(sizeof(int) * (size_t)cap);
    g_PY = (int *)malloc(sizeof(int) * (size_t)cap);
    g_occ[cell(0, 0)] = 1;
    g_occ[cell(0, 1)] = 1;
    g_cnt = 0;
    g_mode = mode;
    if (mode == 0) {
        g_SX[0] = -1; g_SY[0] = 0; g_PX[0] = 0; g_PY[0] = 0;
        g_SX[1] = 1;  g_SY[1] = 0; g_PX[1] = 0; g_PY[1] = 0;
        rec(2, (int)(n - 1));
    } else {
        g_SX[0] = -1; g_SY[0] = 0; g_PX[0] = 0; g_PY[0] = 0;
        rec(1, (int)((n - 1) / 2));
    }
    free(g_occ); free(g_SX); free(g_SY); free(g_PX); free(g_PY);
    return g_cnt;
}

seq_t a(idx_t n)
{
    uint64_t T, M = 0;
    if (n < 1) return (seq_t)0;
    T = run(n, 0);
    if (n % 2 == 1) M = run(n, 1);
    return (seq_t)((T - M) / 2);
}
