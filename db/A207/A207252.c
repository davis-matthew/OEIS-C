/*
 * A207252  Number of nX6 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207252
    A(n) = number of n×6 binary arrays avoiding
    the horizontal patterns 0 0 1 and 1 0 1
    and the vertical patterns 0 1 0 and 1 0 1.
    The function uses a dynamic‑programming algorithm over
    6‑bit row states.  The first 3 terms are
    a(1) = V   (number of valid rows),
    a(2) = V²  (all pairs of rows),
    a(n) (n ≥ 3) is obtained by multiplying the state
    transition matrix.  No explicit formula is known.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Destination list for state transitions */
typedef struct {
    size_t cnt;                 /* number of destinations */
    size_t cap;                 /* capacity of data array */
    uint32_t *data;             /* array of destination state indices */
} DestList;

/* Static data kept between calls */
static bool init_done = false;
static idx_t cached_n = 0;           /* largest n for which a_cache is filled */
static seq_t *a_cache = NULL;        /* cache of a[1..cached_n] */
static size_t a_cache_cap = 0;       /* capacity of a_cache */
static size_t V = 0;                 /* number of valid rows */
static int *valid_rows = NULL;       /* bit patterns of valid rows */
static size_t S = 0;                 /* number of states = V*V */
static DestList *trans = NULL;       /* list of transitions per state */
static seq_t *state_cnt0 = NULL;     /* counts for current step */
static seq_t *state_cnt1 = NULL;     /* counts for next step */

/* ------------------------------------------------------------------ */
/* Check whether a 6‑bit row avoids 001 and 101 horizontally          */
/* ------------------------------------------------------------------ */
static inline bool is_valid_row(int r)
{
    for (int shift = 0; shift <= 3; ++shift) {
        int sub = (r >> shift) & 7;      /* three consecutive bits */
        if (sub == 1 || sub == 5)        /* 001 or 101 */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Ensure the state machinery is built and a_cache is filled up to n  */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (!init_done) {
        /* --- Build valid rows ------------------------------------------------ */
        int tmp[64];
        size_t cnt = 0;
        for (int r = 0; r < 64; ++r)
            if (is_valid_row(r))
                tmp[cnt++] = r;

        V = cnt;
        valid_rows = (int *)malloc(V * sizeof(int));
        memcpy(valid_rows, tmp, V * sizeof(int));

        /* --- Build state graph ------------------------------------------------ */
        S = V * V;
        trans = (DestList *)calloc(S, sizeof(DestList));

        for (size_t i = 0; i < V; ++i) {
            int a = valid_rows[i];
            for (size_t j = 0; j < V; ++j) {
                int b = valid_rows[j];
                size_t src = i * V + j;
                DestList *list = &trans[src];

                for (size_t k = 0; k < V; ++k) {
                    int c = valid_rows[k];
                    bool ok = true;

                    /* Check vertical patterns for this triple of rows */
                    for (int col = 0; col < 6; ++col) {
                        int aa = (a >> col) & 1;
                        int bb = (b >> col) & 1;
                        int cc = (c >> col) & 1;
                        if (aa == 0 && bb == 1 && cc == 0) { ok = false; break; }
                        if (aa == 1 && bb == 0 && cc == 1) { ok = false; break; }
                    }
                    if (ok) {
                        if (list->cnt == list->cap) {
                            size_t newcap = (list->cap == 0) ? 4 : list->cap * 2;
                            list->data = (uint32_t *)realloc(list->data,
                                                              newcap * sizeof(uint32_t));
                            list->cap = newcap;
                        }
                        list->data[list->cnt++] = j * V + k;
                    }
                }
            }
        }

        /* --- Allocate state count arrays ------------------------------------- */
        state_cnt0 = (seq_t *)calloc(S, sizeof(seq_t));
        state_cnt1 = (seq_t *)calloc(S, sizeof(seq_t));

        /* For n = 2 each pair of rows is allowed */
        for (size_t i = 0; i < S; ++i)
            state_cnt0[i] = 1;

        /* --- Allocate cache for sequence values ----------------------------- */
        a_cache_cap = 32;
        a_cache = (seq_t *)calloc(a_cache_cap, sizeof(seq_t));

        /* a(1) = number of valid rows, a(2) = V² (all pairs) */
        a_cache[1] = (seq_t)V;
        a_cache[2] = (seq_t)V * (seq_t)V;
        cached_n = 2;
        init_done = true;
    }

    /* --- Extend cache up to requested n ----------------------------------- */
    if (n <= cached_n)
        return;

    if ((size_t)n + 1 > a_cache_cap) {
        size_t newcap = a_cache_cap ? a_cache_cap * 2 : 32;
        while ((size_t)n + 1 > newcap)
            newcap *= 2;
        a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        a_cache_cap = newcap;
    }

    /* DP iterations for step = cached_n+1 .. n */
    for (idx_t step = cached_n + 1; step <= n; ++step) {
        /* Zero next counts */
        memset(state_cnt1, 0, S * sizeof(seq_t));

        /* Transition */
        for (size_t src = 0; src < S; ++src) {
            seq_t val = state_cnt0[src];
            if (val == 0)
                continue;
            DestList *list = &trans[src];
            for (size_t t = 0; t < list->cnt; ++t)
                state_cnt1[list->data[t]] += val;
        }

        /* Sum to get a(step) */
        seq_t sum = 0;
        for (size_t i = 0; i < S; ++i)
            sum += state_cnt1[i];
        a_cache[step] = sum;

        /* Swap current/next */
        seq_t *tmp = state_cnt0;
        state_cnt0 = state_cnt1;
        state_cnt1 = tmp;
    }

    cached_n = n;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* out‑of‑range guard – never used in this problem */
        return 0;
    ensure_cache(n);
    return a_cache[n];
}
