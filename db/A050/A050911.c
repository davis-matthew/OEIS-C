/*
 * A050911  Number of pure 2-complexes on 6 unlabeled nodes with n multiple
 *          2-simplexes.
 * https://oeis.org/A050911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/|S6| * Σ_{g∈S6} p_g(n) where p_g(n) = # { (c1,…,ck) ≥ 0
 *            | Σ L_j·c_j = n } with L_j the orbit lengths of the induced
 *            action of g on the 20 triangles. The implementation enumerates
 *            the 11 conjugacy classes of S6, precomputes their orbit length
 *            multisets, then for a given n builds the coin‑...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 26, u64 n <= 105, u128 n <= 500,
 *            big any n
 * Helpers:   sort3, sort_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sort3.h"
#include "../utils/sort_uint8.h"

#define NUM_TRIANGLES 20

/* 3‑subsets of {0,…,5} – 20 triangles */
static const int TRI[NUM_TRIANGLES][3] = {
    {0,1,2},{0,1,3},{0,1,4},{0,1,5},
    {0,2,3},{0,2,4},{0,2,5},
    {0,3,4},{0,3,5},
    {0,4,5},
    {1,2,3},{1,2,4},{1,2,5},
    {1,3,4},{1,3,5},
    {1,4,5},
    {2,3,4},{2,3,5},
    {2,4,5},{3,4,5}
};

/* lookup for sorted triple -> triangle index, –1 if not a triangle */
static int TRI_INDEX[6][6][6];

static void init_tri_index(void)
{
    int i, j, k;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 6; ++j)
            for (k = 0; k < 6; ++k)
                TRI_INDEX[i][j][k] = -1;
    for (int t = 0; t < NUM_TRIANGLES; ++t)
        TRI_INDEX[TRI[t][0]][TRI[t][1]][TRI[t][2]] = t;
}

/*  permutation of the 6 vertices */
static int PERM[6];

/* map a triangle index under current permutation */
static int permute_triangle(int t)
{
    int a = TRI[t][0], b = TRI[t][1], c = TRI[t][2];
    int na = PERM[a], nb = PERM[b], nc = PERM[c];
    sort3(&na, &nb, &nc);
    return TRI_INDEX[na][nb][nc];
}

/* information about a conjugacy class of S6 with respect to triangles */
struct ClassInfo
{
    unsigned int size;                /* number of permutations in the class */
    uint8_t       orbit_cnt;          /* how many distinct orbits of triangles */
    uint8_t       orbit_len[NUM_TRIANGLES];   /* orbit lengths (sorted) */
};

#define MAX_CLASSES 11
static struct ClassInfo CLASSES[MAX_CLASSES];
static unsigned int CLASS_COUNT = 0;
static bool CLASSES_INITIALIZED = false;

/* recursive enumeration of all 720 permutations of 0..5 */
static void permute_rec(int pos, int *used)
{
    if (pos == 6)
    {
        /* compute orbit lengths of triangles under current permutation */
        bool vis_tri[NUM_TRIANGLES] = {false};
        uint8_t lens[NUM_TRIANGLES];
        uint8_t lh = 0;
        for (int t = 0; t < NUM_TRIANGLES; ++t)
            if (!vis_tri[t])
            {
                int cur = t, len = 0;
                do
                {
                    vis_tri[cur] = true;
                    cur = permute_triangle(cur);
                    ++len;
                } while (cur != t);
                lens[lh++] = (uint8_t)len;
            }
        sort_uint8(lens, lh);

        /* find or create the corresponding class */
        unsigned int cls = 0;
        while (cls < CLASS_COUNT)
        {
            if (CLASSES[cls].orbit_cnt == lh)
            {
                bool same = true;
                for (int i = 0; i < lh; ++i)
                    if (CLASSES[cls].orbit_len[i] != lens[i])
                    {
                        same = false;
                        break;
                    }
                if (same) break;
            }
            ++cls;
        }
        if (cls == CLASS_COUNT)   /* new class */
        {
            if (CLASS_COUNT >= MAX_CLASSES) return;
            CLASSES[CLASS_COUNT].size        = 0;
            CLASSES[CLASS_COUNT].orbit_cnt   = lh;
            for (int i = 0; i < lh; ++i)
                CLASSES[CLASS_COUNT].orbit_len[i] = lens[i];
            cls = CLASS_COUNT++;
        }
        CLASSES[cls].size += 1;
        return;
    }
    for (int v = 0; v < 6; ++v)
    {
        if (!used[v])
        {
            used[v] = 1;
            PERM[pos] = v;
            permute_rec(pos + 1, used);
            used[v] = 0;
        }
    }
}

static void init_classes(void)
{
    if (CLASSES_INITIALIZED) return;
    init_tri_index();
    int used[6] = {0};
    permute_rec(0, used);
    CLASSES_INITIALIZED = true;
}

/* --------------------------------------------------------------------------- */
/* Result function                                                           */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_classes();
    int N = (int)n;
    /* temporary large integer type for safe intermediate arithmetic */
    /* use 128‑bit if available, otherwise fall back to seq_t (works when seq_t
       itself is arbitrary precision)                                       */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 bigint_t;
#else
    typedef seq_t bigint_t;
#endif

    bigint_t total = 0;
    const bigint_t denom = 720;

    /* buffer for one class DP table */
    bigint_t *dp = (bigint_t*)calloc(N + 1, sizeof(bigint_t));
    if (!dp) return 0;   /* memory error, return zero */

    for (unsigned int ci = 0; ci < CLASS_COUNT; ++ci)
    {
        const struct ClassInfo *c = &CLASSES[ci];
        dp[0] = 1;
        for (int i = 1; i <= N; ++i) dp[i] = 0;

        for (uint8_t oi = 0; oi < c->orbit_cnt; ++oi)
        {
            int w = c->orbit_len[oi];
            for (int s = w; s <= N; ++s)
                dp[s] += dp[s - w];
        }
        bigint_t contrib = (bigint_t)c->size * dp[N];
        total += contrib;
    }

    free(dp);
    seq_t result = (seq_t)(total / denom);
    return result;
}
