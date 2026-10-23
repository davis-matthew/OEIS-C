/*
 * A183693  Number of (n+1)X5 0..2 arrays with every 2X2 subblock nonsingular.
 * https://oeis.org/A183693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 matrices over {0,1,2} * with every 2×2
 *            subblock nonsingular. The sequence is computed * by dynamic
 *            programming on the rows. * * The algorithm builds the 243×243
 *            adjacency matrix of all 5‑digit * states (base‑3 rows). For each
 *            pair of rows s,t it checks that * every of the...
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The constants used below are independent of the seq_t/idx_t types. */
#define STATE_COUNT 243            /* 3^5 possible rows */
#define MAX_NEIGH 243              /* worst‑case adjacency size per state */

/* --------------------------------------------------------------------- */
/*  Build adjacency of valid 2×2 blocks                                         */
/* --------------------------------------------------------------------- */
static unsigned char state_ok[STATE_COUNT][STATE_COUNT];

static void build_adjacency(void)
{
    uint8_t rows[STATE_COUNT][5];
    /* pre‑compute all rows as base‑3 digits */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int t = s;
        for (int d = 4; d >= 0; --d) {
            rows[s][d] = t % 3;
            t /= 3;
        }
    }

    /* check every pair of rows */
    for (int s = 0; s < STATE_COUNT; ++s) {
        const uint8_t *r1 = rows[s];
        for (int t = 0; t < STATE_COUNT; ++t) {
            const uint8_t *r2 = rows[t];
            unsigned char ok = 1;
            for (int j = 0; j < 4; ++j) {
                int det = (int)r1[j] * (int)r2[j+1]
                        - (int)r1[j+1] * (int)r2[j];
                if (det == 0) {
                    ok = 0;
                    break;
                }
            }
            state_ok[s][t] = ok;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Cache for computed sequence values                                      */
/* --------------------------------------------------------------------- */
static seq_t *a_cache   = NULL;   /* 1-based indexing, a_cache[0] unused */
static size_t cache_cap = 0;      /* allocated size of a_cache */
static size_t cache_max = 0;      /* largest n for which a_cache[n] is valid */

/* --------------------------------------------------------------------- */
/*  Dynamic programming: arrays of counts for each state                  */
/* --------------------------------------------------------------------- */
static seq_t cur[STATE_COUNT];
static seq_t nxt[STATE_COUNT];
static unsigned short neigh_idx[STATE_COUNT][MAX_NEIGH];
static unsigned char  neigh_cnt[STATE_COUNT];

/* --------------------------------------------------------------------- */
/*  Ensure that the adjacency information and cache are constructed      */
/* --------------------------------------------------------------------- */
static bool initialized = false;

static void init_once(void)
{
    if (initialized)
        return;
    build_adjacency();

    /* build adjacency lists for fast multiplication */
    for (int s = 0; s < STATE_COUNT; ++s) {
        unsigned char cnt = 0;
        for (int t = 0; t < STATE_COUNT; ++t) {
            if (state_ok[s][t]) {
                neigh_idx[s][cnt++] = (unsigned short)t;
            }
        }
        neigh_cnt[s] = cnt;
    }

    /* initial vector: one row of any type */
    for (int s = 0; s < STATE_COUNT; ++s)
        cur[s] = 1;

    /* allocate cache for a few terms */
    cache_cap = 1024;   /* start with some room */
    a_cache   = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    a_cache[0] = 0;     /* unused */

    cache_max = 0;      /* no terms computed yet */
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Grow the cache array to at least size (n+1)                           */
/* --------------------------------------------------------------------- */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();  /* out of memory */
    /* zero newly added entries */
    for (size_t i = cache_cap; i < new_cap; ++i)
        newp[i] = 0;
    a_cache   = newp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Expand the DP to compute up to height       (n+1) = current_height   */
/* --------------------------------------------------------------------- */
static void compute_up_to(idx_t n)
{
    /* n is the index in the OEIS sequence (starting at 1)            */
    /* We need to compute arrays of height h = n+1                    */
    /* current_height is the height that cur currently represents    */
    static size_t current_height = 1;   /* initially height=1 (one row) */

    init_once();
    ensure_cache_size(n);

    while (current_height <= (size_t)n + 1) {
        /* compute next vector for height = current_height + 1 */
        for (int t = 0; t < STATE_COUNT; ++t)
            nxt[t] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == 0)
                continue;
            unsigned char cnt = neigh_cnt[s];
            unsigned short *list = neigh_idx[s];
            for (unsigned char j = 0; j < cnt; ++j)
                nxt[list[j]] += val;
        }

        /* sum all entries to get a(current_height - 1)             */
        /* because after this step, we have arrays of height        */
        /* current_height + 1 which corresponds to index n =      */
        /* (current_height+1)-1 = current_height.                  */
        seq_t sum = 0;
        for (int t = 0; t < STATE_COUNT; ++t)
            sum += nxt[t];

        /* store a value for n = current_height                */
        a_cache[current_height] = sum;
        if (current_height > cache_max)
            cache_max = current_height;

        /* prepare for next iteration */
        for (int t = 0; t < STATE_COUNT; ++t)
            cur[t] = nxt[t];
        ++current_height;
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but safe guard */

    compute_up_to(n);
    return a_cache[n];
}
