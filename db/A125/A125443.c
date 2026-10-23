/*
 * A125443  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of base 31 circular n‑digit numbers with adjacent
 *            digits differing by 7 or less
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Static adjacency data */
static int neighbor[31][15];
static int neigh_cnt[31];
static int initialized = 0;

/* DP tables */
static seq_t dp_cur[31][31];
static seq_t dp_next[31][31];
static idx_t cur_len = 0;

/* ---- memoisation ---------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static size_t max_computed = 0;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
#endif

/* ---- adjacency initialisation --------------------------------------- */
static void init_adj(void)
{
    for (int i = 0; i < 31; ++i) {
        neigh_cnt[i] = 0;
        for (int d = -7; d <= 7; ++d) {
            int j = i + d;
            if (0 <= j && j < 31) {
                neighbor[i][neigh_cnt[i]++] = j;
            }
        }
    }
}

/* ---- cache (and dp) initialisation --------------------------------- */
static void init_cache(void)
{
    if (initialized)
        return;
    init_adj();

    /* initialise dp_cur as identity (length 0) */
    for (int s = 0; s < 31; ++s) {
        for (int t = 0; t < 31; ++t) {
            dp_cur[s][t] = (s == t) ? (seq_t)1 : (seq_t)0;
        }
    }
    cur_len = 0;

    /* initialise cache */
#ifdef __cplusplus
    cache_vec.resize(2);
    cache_vec[0] = (seq_t)1;           /* a(0) */
    cache_vec[1] = (seq_t)31;          /* a(1) */
    max_computed = 1;
#else
    cache_cap = 2;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    assert(cache != NULL);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)31;
    max_computed = 1;
#endif
    initialized = 1;
}

/* ---- ensure values up to n are computed ----------------------------- */
static void ensure_up_to(idx_t n)
{
    init_cache();

    while (max_computed < n) {
        /* reset dp_next */
        for (int s = 0; s < 31; ++s)
            for (int t = 0; t < 31; ++t)
                dp_next[s][t] = (seq_t)0;

        /* extend one step */
        for (int s = 0; s < 31; ++s) {
            for (int u = 0; u < 31; ++u) {
                seq_t val = dp_cur[s][u];
                if (val == (seq_t)0)
                    continue;
                for (int k = 0; k < neigh_cnt[u]; ++k) {
                    int t = neighbor[u][k];
                    dp_next[s][t] += val;
                }
            }
        }

        /* copy dp_next to dp_cur */
        for (int s = 0; s < 31; ++s)
            for (int t = 0; t < 31; ++t)
                dp_cur[s][t] = dp_next[s][t];

        ++cur_len;                         /* new path length in dp_cur */
        idx_t new_n = cur_len + 1;          /* computed a(new_n) */

        /* count cycles: start at s, end at t, with t adjacent to s */
        seq_t total = (seq_t)0;
        for (int s = 0; s < 31; ++s) {
            for (int k = 0; k < neigh_cnt[s]; ++k) {
                int t = neighbor[s][k];
                total += dp_cur[s][t];
            }
        }

        /* store result */
#ifdef __cplusplus
        cache_vec.push_back(total);
#else
        if (new_n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)new_n)
                new_cap = (size_t)new_n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            assert(tmp != NULL);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[new_n] = total;
#endif
        max_computed = new_n;
    }
}

/* ------------------- public function --------------------------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);
#ifdef __cplusplus
    return cache_vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
