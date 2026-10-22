/*
 * A003752  Number of Hamiltonian paths in C_4 X P_n.
 * https://oeis.org/A003752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   transfer-matrix count over the columns of C_4 X P_n. Each state
 *            records which of the 4 cycle vertices send an edge to the next
 *            column and how those path pieces are connected. a(n) = sum over
 *            final states after n-1 middle-column steps starting from the
 *            empty state
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int tr_ready = 0;
static int st_list[256], st_cnt = 0;
static int tr_from[65536], tr_to[65536], tr_mul[65536], tr_n = 0;
static int fin_mul[256];

static int uf_find(int *p, int x)
{
    while (p[x] != x) x = p[x];
    return x;
}

/* returns new state, or -1 if invalid; for fin, returns 0 when it completes a single path */
static int col_step(int s, int E, int O, int fin)
{
    int lab[4], deg[4], cnt[4] = {0, 0, 0, 0}, par[8], pc[8], seen[8], map[8];
    int i, e, L, ends = 0, singles = 0, ncomp = 0, nxt = 1, t = 0;
    for (i = 0; i < 4; i++) { lab[i] = (s >> (2 * i)) & 3; cnt[lab[i]]++; }
    for (i = 0; i < 4; i++) {
        deg[i] = (lab[i] != 0) + ((O >> i) & 1) + ((E >> i) & 1) + ((E >> ((i + 3) & 3)) & 1);
        if (deg[i] < 1 || deg[i] > 2) return -1;
        if (deg[i] == 1) ends++;
    }
    for (L = 1; L < 4; L++) {
        if (cnt[L] > 2) return -1;
        if (cnt[L] == 1) singles++;
    }
    if (singles + ends > 2) return -1;
    for (i = 0; i < 8; i++) { par[i] = i; pc[i] = 0; seen[i] = 0; map[i] = 0; }
    for (e = 0; e < 4; e++) {
        if ((E >> e) & 1) {
            int a = uf_find(par, e), b = uf_find(par, (e + 1) & 3);
            if (a == b) return -1;
            par[a] = b;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lab[i]) {
            int a = uf_find(par, i), b = uf_find(par, 4 + lab[i]);
            if (a == b) return -1;
            par[a] = b;
        }
    }
    for (i = 0; i < 4; i++) {
        int r = uf_find(par, i);
        if (!seen[r]) { seen[r] = 1; ncomp++; }
        if ((O >> i) & 1) pc[r]++;
    }
    if (fin) return ncomp == 1 ? 0 : -1;
    for (i = 0; i < 4; i++)
        if (pc[uf_find(par, i)] == 0) return -1;
    for (i = 0; i < 4; i++) {
        if ((O >> i) & 1) {
            int r = uf_find(par, i);
            if (!map[r]) map[r] = nxt++;
            t |= map[r] << (2 * i);
        }
    }
    return t;
}

static void build_transitions(void)
{
    static int reach[256];
    static int cntm[256];
    int qi, s, E, O, t, k;
    for (s = 0; s < 256; s++) { reach[s] = 0; fin_mul[s] = 0; }
    reach[0] = 1;
    st_list[st_cnt++] = 0;
    for (qi = 0; qi < st_cnt; qi++) {
        s = st_list[qi];
        for (t = 0; t < 256; t++) cntm[t] = 0;
        for (E = 0; E < 16; E++) {
            if (col_step(s, E, 0, 1) == 0) fin_mul[s]++;
            for (O = 0; O < 16; O++) {
                t = col_step(s, E, O, 0);
                if (t >= 0) cntm[t]++;
            }
        }
        for (t = 0; t < 256; t++) {
            if (cntm[t]) {
                tr_from[tr_n] = s; tr_to[tr_n] = t; tr_mul[tr_n] = cntm[t]; tr_n++;
                if (!reach[t]) { reach[t] = 1; st_list[st_cnt++] = t; }
            }
        }
    }
    (void)k;
    tr_ready = 1;
}

static seq_t cache_v[256];
static seq_t tmp_v[256];
static idx_t cache_k = 0;
static int cache_ok = 0;

seq_t a(idx_t n)
{
    int i, j;
    seq_t res;
    if (!tr_ready) build_transitions();
    if (!cache_ok || n - 1 < cache_k) {
        for (i = 0; i < 256; i++) cache_v[i] = 0;
        cache_v[0] = 1;
        cache_k = 0;
        cache_ok = 1;
    }
    while (cache_k < n - 1) {
        for (i = 0; i < st_cnt; i++) tmp_v[st_list[i]] = 0;
        for (j = 0; j < tr_n; j++)
            tmp_v[tr_to[j]] += cache_v[tr_from[j]] * (seq_t)tr_mul[j];
        for (i = 0; i < st_cnt; i++) cache_v[st_list[i]] = tmp_v[st_list[i]];
        cache_k++;
    }
    res = 0;
    for (i = 0; i < st_cnt; i++) {
        int s = st_list[i];
        if (fin_mul[s]) res += cache_v[s] * (seq_t)fin_mul[s];
    }
    return res;
}
