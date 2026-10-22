/*
 * A006783  Maximally extended polygons of length 2n on cubic lattice.
 * https://oeis.org/A006783
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   no closed form; computed by exhaustive search. The code counts
 *            self-avoiding polygons of length 2n on Z^3 whose total extent
 *            (width+height+depth) is n, so each coordinate rises once and
 *            falls once around the cycle. Each polygon is rooted at its
 *            lexicographically least vertex, and one of its two o...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int g_n, g_w;
static unsigned char *g_vis;
static int g_px, g_py, g_pz;
static int g_sg[3], g_ch[3];
static int g_first;
static unsigned long long g_cnt;

static const int DXS[6] = {1, -1, 0, 0, 0, 0};
static const int DYS[6] = {0, 0, 1, -1, 0, 0};
static const int DZS[6] = {0, 0, 0, 0, 1, -1};
static const int LIMS[3] = {1, 2, 2};

static void a006783_dfs(int r)
{
    int d;
    for (d = 0; d < 6; d++) {
        int c = d >> 1;
        int sg = (d & 1) ? -1 : 1;
        int os = g_sg[c], och = g_ch[c];
        int nch = och;
        int nx, ny, nz, dist;
        long idx;
        if (os != 0 && os != sg) {
            nch++;
            if (nch > LIMS[c]) continue;
        }
        nx = g_px + DXS[d];
        ny = g_py + DYS[d];
        nz = g_pz + DZS[d];
        if (r == 1) {
            if (nx == 0 && ny == 0 && nz == 0 && c < g_first) g_cnt++;
            continue;
        }
        /* every non-root vertex must be lexicographically greater than root */
        if (!(nx > 0 || (nx == 0 && (ny > 0 || (ny == 0 && nz > 0))))) continue;
        dist = nx + (ny < 0 ? -ny : ny) + (nz < 0 ? -nz : nz);
        if (dist > r - 1) continue;
        if (nx > g_n || ny > g_n || ny < -g_n || nz > g_n || nz < -g_n) continue;
        idx = ((long)nx * g_w + (ny + g_n)) * g_w + (nz + g_n);
        if (g_vis[idx]) continue;
        g_vis[idx] = 1;
        g_sg[c] = sg;
        g_ch[c] = nch;
        g_px = nx; g_py = ny; g_pz = nz;
        a006783_dfs(r - 1);
        g_px -= DXS[d]; g_py -= DYS[d]; g_pz -= DZS[d];
        g_sg[c] = os;
        g_ch[c] = och;
        g_vis[idx] = 0;
    }
}

static unsigned long long a006783_count(int n)
{
    long size;
    int f;
    g_n = n;
    g_w = 2 * n + 1;
    size = (long)(n + 1) * g_w * g_w;
    g_vis = (unsigned char *)malloc((size_t)size);
    memset(g_vis, 0, (size_t)size);
    g_cnt = 0;
    g_vis[((long)0 * g_w + n) * g_w + n] = 1; /* root at origin */
    /* first step goes to +z (c=2) or +y (c=1); last step must come from a
       neighbour of lower coordinate index, fixing one orientation */
    for (f = 1; f <= 2; f++) {
        long idx;
        g_sg[0] = g_sg[1] = g_sg[2] = 0;
        g_ch[0] = g_ch[1] = g_ch[2] = 0;
        g_px = 0; g_py = (f == 1) ? 1 : 0; g_pz = (f == 2) ? 1 : 0;
        g_sg[f] = 1;
        g_first = f;
        idx = ((long)g_px * g_w + (g_py + n)) * g_w + (g_pz + n);
        g_vis[idx] = 1;
        a006783_dfs(2 * n - 1);
        g_vis[idx] = 0;
    }
    free(g_vis);
    g_vis = NULL;
    return g_cnt;
}

seq_t a(idx_t n)
{
    static unsigned long long memo[64];
    static bool have[64];
    unsigned long long v;
    if (n < 2) return (seq_t)0;
    if (n < 64 && have[n]) return (seq_t)memo[n];
    v = a006783_count((int)n);
    if (n < 64) {
        memo[n] = v;
        have[n] = true;
    }
    return (seq_t)v;
}
