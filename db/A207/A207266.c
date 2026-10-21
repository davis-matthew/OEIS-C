/*
 * A207266  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding the horizontal
 *            patterns 000 and 011, and the vertical patterns 0 1 1 and 1 0 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


/* Static tables used to compute the recurrence described above. */
struct TransList {
    uint8_t *items;    /* list of allowed third rows */
    size_t   len;      /* current number of items */
    size_t   cap;      /* allocated capacity */
};

static bool        initialized = false;
static uint8_t     n_allowed;     /* number of allowed 5‑bit rows */
static uint8_t    *allowed;       /* array of permissible rows */
static struct TransList *trans;   /* transition lists for each (a,b) pair */

static seq_t   *dp_cur;        /* DP table for current length   */
static size_t   dp_size;       /* (n_allowed)^2                 */
static idx_t    cur_len;       /* length for which dp_cur holds */

static seq_t   *total_cache;   /* a(n) values, 1‑based index   */
static size_t   total_cap;     /* allocated capacity of cache  */
static idx_t    total_len;     /* largest n cached              */

/* ------------------------------------------------------------------ */
/* Check whether a 5‑bit pattern satisfies the horizontal constraints. */
static bool horizontal_ok(uint8_t pat)
{
    for (int j = 0; j < 3; ++j) {
        int b0 = (pat >> j    ) & 1;
        int b1 = (pat >> (j+1)) & 1;
        int b2 = (pat >> (j+2)) & 1;
        if (b0 == 0 && b1 == 0 && b2 == 0) return false; /* 000 */
        if (b0 == 0 && b1 == 1 && b2 == 1) return false; /* 011 */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Initialise all static data on first call. */
static void init(void)
{
    if (initialized) return;

    /* 1. Enumerate allowed 5‑bit rows. */
    uint8_t pats[32];
    size_t cnt = 0;
    for (uint8_t p = 0; p < 32; ++p)
        if (horizontal_ok(p)) pats[cnt++] = p;

    n_allowed = (uint8_t)cnt;
    allowed = (uint8_t *)malloc(n_allowed * sizeof(uint8_t));
    for (size_t i = 0; i < n_allowed; ++i) allowed[i] = pats[i];

    /* 2. Build transition lists for every pair (a,b). */
    size_t pair_cnt = n_allowed * n_allowed;
    trans = (struct TransList *)malloc(pair_cnt * sizeof(struct TransList));
    for (size_t i = 0; i < pair_cnt; ++i) {
        trans[i].items = NULL;
        trans[i].len   = 0;
        trans[i].cap   = 0;
    }

    for (size_t ai = 0; ai < n_allowed; ++ai) {
        uint8_t a = allowed[ai];
        for (size_t bi = 0; bi < n_allowed; ++bi) {
            uint8_t b = allowed[bi];
            size_t idx = ai * n_allowed + bi;
            for (size_t ci = 0; ci < n_allowed; ++ci) {
                uint8_t c = allowed[ci];
                /* vertical constraints: 0 1 1 and 1 0 1 */
                bool bad = false;
                for (int j = 0; j < 5; ++j) {
                    int it = (a >> j) & 1;
                    int bt = (b >> j) & 1;
                    int ct = (c >> j) & 1;
                    if (it == 0 && bt == 1 && ct == 1) { bad = true; break; }
                    if (it == 1 && bt == 0 && ct == 1) { bad = true; break; }
                }
                if (!bad) {
                    struct TransList *t = &trans[idx];
                    if (t->len == t->cap) {
                        size_t newcap = t->cap ? t->cap * 2 : 4;
                        t->items = (uint8_t *)realloc(t->items, newcap * sizeof(uint8_t));
                        t->cap   = newcap;
                    }
                    t->items[t->len++] = (uint8_t)ci;
                }
            }
        }
    }

    /* 3. Initialise DP for length 2 (every pair of allowed rows). */
    dp_size = (size_t)n_allowed * n_allowed;
    dp_cur  = (seq_t *)calloc(dp_size, sizeof(seq_t));
    for (size_t ai = 0; ai < n_allowed; ++ai)
        for (size_t bi = 0; bi < n_allowed; ++bi)
            dp_cur[ai * n_allowed + bi] = (seq_t)1;

    cur_len = 2;

    /* 4. Total cache: store a(1) and a(2). */
    total_cap   = 64;
    total_cache = (seq_t *)calloc(total_cap + 1, sizeof(seq_t)); /* 1‑based index */
    total_len   = 2;

    total_cache[1] = (seq_t)n_allowed;
    total_cache[2] = (seq_t)n_allowed * (seq_t)n_allowed;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    init();

    if (n == 0) return (seq_t)0;      /* offset is 1 */
    if (n <= total_len) return total_cache[(size_t)n];

    /* Extend DP up to requested length. */
    while (cur_len < n) {
        seq_t *newdp = (seq_t *)calloc(dp_size, sizeof(seq_t));

        for (size_t ai = 0; ai < n_allowed; ++ai) {
            for (size_t bi = 0; bi < n_allowed; ++bi) {
                seq_t val = dp_cur[ai * n_allowed + bi];
                if (val == 0) continue;
                struct TransList *t = &trans[ai * n_allowed + bi];
                for (size_t k = 0; k < t->len; ++k) {
                    uint8_t ci = t->items[k];
                    newdp[bi * n_allowed + ci] += val;
                }
            }
        }

        free(dp_cur);
        dp_cur = newdp;
        ++cur_len;

        /* Compute total for this length. */
        seq_t sum = 0;
        for (size_t i = 0; i < dp_size; ++i) sum += dp_cur[i];

        if (cur_len > total_len) {
            if (cur_len > total_cap) {
                size_t newcap = total_cap ? total_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(total_cache, (newcap + 1) * sizeof(seq_t));
                total_cache = tmp;
                total_cap   = newcap;
            }
            total_cache[cur_len] = sum;
            total_len = cur_len;
        }
    }

    return total_cache[(size_t)n];
}
