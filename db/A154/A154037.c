/*
 * A154037  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          3 X 3 X 3 subtriangle summing to 4.
 * https://oeis.org/A154037
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of planar triangular n×n×n nonnegative
 *            integer * grids symmetric under 120° rotation with every
 *            similarly oriented 3×3×3 * subtriangle summing to 4. For n ≥ 11
 *            the sequence follows the pattern * 0,0,27 repeating (n mod 3==1
 *            ⇒ 27, otherwise 0). For n < 11 it is * computed by exh...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------- helper for the exhaustive search of n < 11 ------------- */

#define MAX_N      10
#define MAX_POINTS (MAX_N * (MAX_N + 1) / 2)
#define MAX_ORBITS 30
#define MAX_TRIS   50
#define MAX_T_PER_ORB 30

/* global arrays used by the recursive search */
static int  orbit_order[MAX_ORBITS];
static int  rem_arr[MAX_TRIS];
static int  maxrem_arr[MAX_TRIS];
static int  tri_count_glob;
static int  orbit_tri_cnt[MAX_ORBITS];
static int  orbit_tri_id[MAX_ORBITS][MAX_T_PER_ORB];
static int  orbit_tri_coeff[MAX_ORBITS][MAX_T_PER_ORB];

/* depth‑first search for the number of solutions */
static seq_t dfs_small(int pos, int orbit_cnt)
{
    if (pos == orbit_cnt) {
        for (int t = 0; t < tri_count_glob; ++t)
            if (rem_arr[t] != 0) return 0;
        return 1;
    }
    int o = orbit_order[pos];
    seq_t total = 0;
    int cnt_tri = orbit_tri_cnt[o];
    int *tri_ids = orbit_tri_id[o];
    int *tri_coeff = orbit_tri_coeff[o];

    for (int v = 0; v <= 4; ++v) {          /* 0≤value≤4 is forced by the sum */
        bool ok = true;
        for (int i = 0; i < cnt_tri; ++i) {
            int t = tri_ids[i];
            int c = tri_coeff[i];
            rem_arr[t] -= c * v;
            maxrem_arr[t] -= 4 * c;
            if (rem_arr[t] < 0 || rem_arr[t] > maxrem_arr[t]) { ok = false; }
        }
        if (ok) total += dfs_small(pos + 1, orbit_cnt);

        /* backtrack */
        for (int i = 0; i < cnt_tri; ++i) {
            int t = tri_ids[i];
            int c = tri_coeff[i];
            rem_arr[t] += c * v;
            maxrem_arr[t] += 4 * c;
        }
    }
    return total;
}

/* count solutions for n < 11 */
static seq_t small_n_count(idx_t n)
{
    int N = (int)n;                    /* N <= 10 */
    int orbit_id[MAX_N][MAX_N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            orbit_id[i][j] = -1;

    int orbit_cnt = 0;
    /* build orbits */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - i; ++j) {
            if (orbit_id[i][j] != -1) continue;
            int k = N - 1 - i - j;
            int pts[3][2] = { {i, j}, {j, k}, {k, i} };
            /* choose lexicographically smallest as canonical */
            int best = 0;
            for (int t = 1; t < 3; ++t)
                if (pts[t][0] < pts[best][0] ||
                   (pts[t][0] == pts[best][0] && pts[t][1] < pts[best][1]))
                    best = t;
            for (int t = 0; t < 3; ++t) {
                int a = pts[t][0], b = pts[t][1];
                orbit_id[a][b] = orbit_cnt;
            }
            ++orbit_cnt;
        }
    }
    /* number of subtriangles of side 3 in a triangle of side N */
    tri_count_glob = (N - 2) * (N - 1) / 2;
    /* triangle data: each triangle has up to 6 different orbits */
    int tri_orbits[MAX_TRIS][6];
    int tri_coeffs[MAX_TRIS][6];
    int tri_sizes[MAX_TRIS] = {0};

    for (int t = 0; t < tri_count_glob; ++t) {
        int ii = 0;
        /* generate all (i,j) with i+j <= N-3, we enumerate in order */
    }
    /* recompute bounds for all (i,j) with i+j <= N-3 */
    int idx = 0;
    for (int i = 0; i <= N - 3; ++i) {
        for (int j = 0; j <= N - 3 - i; ++j, ++idx) {
            int pts[6][2] = {
                {i, j},                 /* top */
                {i + 1, j},             /* down-left */
                {i, j + 1},             /* down-right */
                {i + 2, j},             /* further down-left */
                {i + 1, j + 1},         /* center */
                {i, j + 2}              /* further down-right */
            };
            int orbs[6], coeffs[6];
            int sz = 0;
            for (int p = 0; p < 6; ++p) {
                int a = pts[p][0], b = pts[p][1];
                int o = orbit_id[a][b];
                int kk = 0;
                for (; kk < sz; ++kk)
                    if (orbs[kk] == o) break;
                if (kk < sz) {
                    coeffs[kk]++;
                } else {
                    orbs[kk] = o;
                    coeffs[kk] = 1;
                    ++sz;
                }
            }
            tri_sizes[idx] = sz;
            for (int k = 0; k < sz; ++k) {
                tri_orbits[idx][k] = orbs[k];
                tri_coeffs[idx][k] = coeffs[k];
            }
        }
    }

    /* build orbit → triangle lists */
    for (int o = 0; o < orbit_cnt; ++o) {
        orbit_tri_cnt[o] = 0;
    }
    for (int t = 0; t < tri_count_glob; ++t) {
        int sz = tri_sizes[t];
        for (int k = 0; k < sz; ++k) {
            int o = tri_orbits[t][k];
            int c = tri_coeffs[t][k];
            orbit_tri_id[o][orbit_tri_cnt[o]] = t;
            orbit_tri_coeff[o][orbit_tri_cnt[o]] = c;
            ++orbit_tri_cnt[o];
        }
    }

    /* initialise remaining sums and maximum capacities */
    for (int t = 0; t < tri_count_glob; ++t) {
        rem_arr[t] = 4;
        int max = 0;
        for (int k = 0; k < tri_sizes[t]; ++k)
            max += 4 * tri_coeffs[t][k];
        maxrem_arr[t] = max;
    }

    /* order orbits by descending degree (number of triangles) */
    for (int o = 0; o < orbit_cnt; ++o)
        orbit_order[o] = o;
    /* simple bubble sort */
    for (int i = 0; i < orbit_cnt - 1; ++i)
        for (int j = 0; j < orbit_cnt - i - 1; ++j)
            if (orbit_tri_cnt[orbit_order[j]] < orbit_tri_cnt[orbit_order[j + 1]]) {
                int tmp = orbit_order[j];
                orbit_order[j] = orbit_order[j + 1];
                orbit_order[j + 1] = tmp;
            }

    return dfs_small(0, orbit_cnt);
}

/* ------------- caching and public interface ------------- */

static seq_t cache[10001];
static bool cache_initialized = false;

static void init_cache(void)
{
    for (size_t i = 0; i < 10001; ++i)
        cache[i] = (seq_t)-1;
    cache_initialized = true;
}

seq_t a(idx_t n)
{
    if (!cache_initialized)
        init_cache();

    if (n < 3)
        return 0;                    /* undefined for the offset */

    /* immutable checks for n < 11 */
    if (n < 11) {
        if (cache[n] == (seq_t)-1)
            cache[n] = small_n_count(n);
        return cache[n];
    }

    /* pattern 0,0,27 starting at n=11 */
    if (n % 3 == 1)
        return (seq_t)27;
    return 0;
}
