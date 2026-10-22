/*
 * A006745  Number of indefinitely growing n-step self-avoiding walks on
 *          Manhattan lattice.
 * https://oeis.org/A006745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   no closed form; exhaustive depth-first enumeration of
 *            self-avoiding walks on the Manhattan lattice starting with an
 *            east step. A walk is cut off as soon as its head can no longer
 *            reach infinity (trapped). Trapping is tested by a flood fill,
 *            which is skipped when a local argument shows the head ca...
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   hx, u128_to_seq, vy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/hx.h"
#include "../utils/vy.h"

static uint64_t *cnt_cache = NULL;
static idx_t cache_max = 0;

static int N, OFF, W;
static char *occ;
static unsigned *stamp;
static unsigned cur_stamp;
static int *stk;
static uint64_t *cnt;

static int ID(int x, int y) { return (x + OFF) * W + (y + OFF); }

static bool escapes(int sx, int sy, int minx, int maxx, int miny, int maxy)
{
    int sp = 0;
    cur_stamp++;
    if (cur_stamp == 0) {
        memset(stamp, 0, sizeof(unsigned) * (size_t)W * (size_t)W);
        cur_stamp = 1;
    }
    stamp[ID(sx, sy)] = cur_stamp;
    stk[sp++] = sx; stk[sp++] = sy;
    while (sp > 0) {
        int y = stk[--sp];
        int x = stk[--sp];
        int k;
        for (k = 0; k < 2; k++) {
            int nx = (k == 0) ? hx(x, y) : x;
            int ny = (k == 0) ? y : vy(x, y);
            int id;
            if (nx < minx || nx > maxx || ny < miny || ny > maxy) return true;
            id = ID(nx, ny);
            if (occ[id] || stamp[id] == cur_stamp) continue;
            stamp[id] = cur_stamp;
            stk[sp++] = nx; stk[sp++] = ny;
        }
    }
    return false;
}

static void rec(int x, int y, int depth, int minx, int maxx, int miny, int maxy)
{
    int cx[2], cy[2], k;
    cx[0] = hx(x, y); cy[0] = y;
    cx[1] = x;        cy[1] = vy(x, y);
    for (k = 0; k < 2; k++) {
        int nx = cx[k], ny = cy[k];
        int qx = cx[1 - k], qy = cy[1 - k];
        int id = ID(nx, ny);
        int a1, a2, b1, b2;
        bool safe = false;
        if (occ[id]) continue;
        a1 = nx < minx ? nx : minx;
        a2 = nx > maxx ? nx : maxx;
        b1 = ny < miny ? ny : miny;
        b2 = ny > maxy ? ny : maxy;
        if (nx < minx || nx > maxx || ny < miny || ny > maxy) {
            safe = true;               /* new extreme: escapes outward */
        } else if (occ[ID(qx, qy)]) {
            safe = true;               /* old escape path had to go through nx */
        } else {
            int dx = nx - x, dy = ny - y, ex = qx - x, ey = qy - y;
            if (!occ[ID(nx + dx, ny + dy)] &&
                !occ[ID(nx + dx + ex, ny + dy + ey)] &&
                !occ[ID(nx + ex, ny + ey)])
                safe = true;           /* nx reaches q locally */
        }
        occ[id] = 1;
        if (!safe) safe = escapes(nx, ny, a1, a2, b1, b2);
        if (safe) {
            cnt[depth + 1]++;
            if (depth + 1 < N) rec(nx, ny, depth + 1, a1, a2, b1, b2);
        }
        occ[id] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n > cache_max) {
        int i;
        size_t cells;
        N = (int)n;
        OFF = N + 4;
        W = 2 * OFF + 1;
        cells = (size_t)W * (size_t)W;
        occ = (char *)calloc(cells, 1);
        stamp = (unsigned *)calloc(cells, sizeof(unsigned));
        stk = (int *)malloc(sizeof(int) * 2 * (cells + 4));
        cur_stamp = 0;
        cnt = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 2));
        for (i = 0; i <= N + 1; i++) cnt[i] = 0;
        occ[ID(0, 0)] = 1;             /* origin */
        occ[ID(1, 0)] = 1;             /* first step east to (1,0) */
        cnt[1] = 1;
        if (N > 1) rec(1, 0, 1, 0, 1, 0, 0);
        free(occ); free(stamp); free(stk);
        if (cnt_cache) free(cnt_cache);
        cnt_cache = cnt;
        cache_max = n;
    }
    return u128_to_seq((unsigned __int128)cnt_cache[n]);
}
