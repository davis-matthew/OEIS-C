/*
 * A003737  Number of Hamiltonian cycles in W_5 X P_n.
 * https://oeis.org/A003737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   transfer-matrix count, layer by layer over the n copies of W_5
 *            (hub + 4-cycle rim); state = set of vertical edges used plus
 *            their pairing by paths below; states lumped by the dihedral
 *            symmetry of the rim. a(n) = start * C^(n-2) * final, with a(1) =
 *            number of Hamiltonian cycles of W_5 = 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   a3737_decode, a3737_encode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/a3737_decode.h"
#include "../utils/a3737_encode.h"

#define A3737_MAXS 64

static int a3737_ns, a3737_no;
static int a3737_T[A3737_MAXS][A3737_MAXS];
static int a3737_F[A3737_MAXS];
static int a3737_keys[A3737_MAXS];
static int a3737_orb[A3737_MAXS], a3737_rep[A3737_MAXS], a3737_sz[A3737_MAXS];
static int a3737_C[A3737_MAXS][A3737_MAXS];
static bool a3737_ready = false;

static const int a3737_eu[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static const int a3737_ev[8] = {1, 2, 3, 4, 2, 3, 4, 1};

static int a3737_find(int key)
{
    int i;
    for (i = 0; i < a3737_ns; i++)
        if (a3737_keys[i] == key) return i;
    a3737_keys[a3737_ns] = key;
    return a3737_ns++;
}

static void a3737_init(void)
{
    int s, o, p, canon[A3737_MAXS];
    a3737_ns = 0;
    a3737_find(0);
    for (s = 0; s < a3737_ns; s++) {
        int min[5], em, om;
        a3737_decode(a3737_keys[s], min);
        for (em = 0; em < 256; em++) for (om = 0; om < 32; om++) {
            int adjn[5], adjv[5][2], adje[5][2], nm[5], vis[5], v, e;
            bool ok = true;
            for (v = 0; v < 5; v++) adjn[v] = 0;
            for (e = 0; e < 13 && ok; e++) {
                int u, w;
                if (e < 8) {
                    if (!((em >> e) & 1)) continue;
                    u = a3737_eu[e]; w = a3737_ev[e];
                } else {
                    u = e - 8;
                    if (min[u] <= u) continue;
                    w = min[u];
                }
                if (adjn[u] >= 2 || adjn[w] >= 2) { ok = false; break; }
                adjv[u][adjn[u]] = w; adje[u][adjn[u]] = e; adjn[u]++;
                adjv[w][adjn[w]] = u; adje[w][adjn[w]] = e; adjn[w]++;
            }
            if (!ok) continue;
            for (v = 0; v < 5; v++)
                if (adjn[v] + ((om >> v) & 1) != 2) ok = false;
            if (!ok) continue;
            for (v = 0; v < 5; v++) { nm[v] = -1; vis[v] = 0; }
            for (v = 0; v < 5; v++) {
                if (((om >> v) & 1) && !vis[v]) {
                    int cur = v, pe = -1;
                    vis[v] = 1;
                    for (;;) {
                        int j = (adjn[cur] == 2 && adje[cur][0] == pe) ? 1 : 0;
                        pe = adje[cur][j];
                        cur = adjv[cur][j];
                        vis[cur] = 1;
                        if ((om >> cur) & 1) break;
                    }
                    nm[v] = cur; nm[cur] = v;
                }
            }
            if (om) {
                for (v = 0; v < 5; v++) if (!vis[v]) ok = false;
                if (!ok) continue;
                a3737_T[s][a3737_find(a3737_encode(nm))]++;
            } else {
                int cur = 0, pe = -1, cnt = 0;
                do {
                    int j = (adje[cur][0] == pe) ? 1 : 0;
                    pe = adje[cur][j];
                    cur = adjv[cur][j];
                    cnt++;
                } while (cur != 0);
                if (cnt == 5) a3737_F[s]++;
            }
        }
    }
    for (s = 0; s < a3737_ns; s++) {
        int m[5], best = -1, g;
        a3737_decode(a3737_keys[s], m);
        for (g = 0; g < 8; g++) {
            int pm[5], im[5], v, k;
            pm[0] = 0;
            for (v = 1; v < 5; v++)
                pm[v] = g < 4 ? 1 + ((v - 1 + g) % 4) : 1 + (((g - 4) - (v - 1) + 4) % 4);
            for (v = 0; v < 5; v++) im[pm[v]] = m[v] < 0 ? -1 : pm[m[v]];
            k = a3737_encode(im);
            if (best < 0 || k < best) best = k;
        }
        canon[s] = best;
    }
    a3737_no = 0;
    for (s = 0; s < a3737_ns; s++) {
        for (o = 0; o < a3737_no; o++)
            if (canon[a3737_rep[o]] == canon[s]) break;
        if (o == a3737_no) { a3737_rep[o] = s; a3737_sz[o] = 0; a3737_no++; }
        a3737_orb[s] = o;
        a3737_sz[o]++;
    }
    for (o = 0; o < a3737_no; o++)
        for (p = 0; p < a3737_no; p++) a3737_C[o][p] = 0;
    for (s = 0; s < a3737_ns; s++)
        for (p = 0; p < a3737_no; p++)
            a3737_C[a3737_orb[s]][p] += a3737_T[s][a3737_rep[p]];
    a3737_ready = true;
}

seq_t a(idx_t n)
{
    seq_t w[A3737_MAXS], nw[A3737_MAXS], r;
    idx_t i;
    int o, p;
    if (!a3737_ready) a3737_init();
    if (n == 1) return (seq_t)a3737_F[0];
    for (p = 0; p < a3737_no; p++) w[p] = (seq_t)a3737_T[0][a3737_rep[p]];
    for (i = 2; i < n; i++) {
        for (p = 0; p < a3737_no; p++) {
            nw[p] = 0;
            for (o = 0; o < a3737_no; o++)
                if (a3737_C[o][p]) nw[p] += w[o] * (seq_t)a3737_C[o][p];
        }
        for (p = 0; p < a3737_no; p++) w[p] = nw[p];
    }
    r = 0;
    for (o = 0; o < a3737_no; o++) {
        int f = a3737_sz[o] * a3737_F[a3737_rep[o]];
        if (f) r += w[o] * (seq_t)f;
    }
    return r;
}
