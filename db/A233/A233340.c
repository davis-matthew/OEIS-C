/*
 * A233340  Number of tilings of a 5 X 3n rectangle using 5n trominoes of any
 *          shape.
 * https://oeis.org/A233340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233340 – number of tilings of a 5×3n rectangle with trominoes. */

/* The implementation uses dynamic programming over column states.
 * Each state records the occupancy masks of the current column and the
 * next column.  A transition fills the current column completely and
 * produces a new pair of masks for the next step.  Transition
 * information is pre‑computed once and reused for all calls.  The
 * algorithm runs in O(n⋅S⋅T) time, where S=1024 is the state count
 * and T is the average number of transitions per state.  For the
 * required range (n up to ~10000) this stays well below the time
 * limit. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Configuration constants                                               */
#define STATES        1024          /* 32 (cur) × 32 (next)                 */
#define MAX_TRANS     200           /* maximum transitions per state        */

/* ------------------------------------------------------------------ */
/* Transition structure – one entry per state transition             */
typedef struct
{
    uint16_t next_cur ; /* new 'cur' mask   (5 bits)            */
    uint16_t next_next; /* new 'next' mask (5 bits)            */
    seq_t    count ;    /* number of ways to achieve this step  */
} Transition;

/* ------------------------------------------------------------------ */
/* Tromino shapes – each shape lists three cells relative to an
 * anchor cell.  dr = column offset (0..2), dc = row offset (0..4).      */
static const struct
{
    int dr[3];
    int dc[3];
} shapes[6] =
{
    /* horizontal domino (L‑shaped trominoes rotate)                */
    { {0,0,0}, {0,1,2} },  /* horizontal line (across 3 columns)           */
    { {0,1,2}, {0,0,0} },  /* vertical   line (across 3 rows)             */
    { {0,1,1}, {0,0,1} },  /* L‑shape A (corner at top‑left)             */
    { {0,0,1}, {0,1,0} },  /* L‑shape B (corner at top‑right)            */
    { {0,0,1}, {0,1,1} },  /* L‑shape C (corner at bottom‑left)          */
    { {0,1,1}, {1,0,1} }   /* L‑shape D (corner at bottom‑right)         */
};

/* ------------------------------------------------------------------ */
/* Transition tables – one row per state, variable length.           */
static Transition trans[STATES][MAX_TRANS];
static size_t     trans_cnt[STATES];

/* ------------------------------------------------------------------ */
/* DFS that enumerates all completions of the current column.  It
 * writes the resulting (next, next_next) masks and number of ways
 * into the array 'final'.                                                   */
static void dfs_fill(uint16_t bits[3], seq_t final[STATES])
{
    /* If current column is completely filled: record the state. */
    if ((bits[0] & 0x1F) == 0x1F)
    {
        uint16_t key = ((bits[1] & 0x1F) << 5) | (bits[2] & 0x1F);
        final[key] += (seq_t)1;
        return;
    }

    /* Find the first vacant cell in the current column.            */
    uint16_t freeMask = (~bits[0]) & 0x1F;
    int col = __builtin_ctz((unsigned int)freeMask); /* least free row */

    /* Try all shapes and anchor positions.                          */
    for (int s = 0; s < 6; ++s)
    {
        for (int a = 0; a < 3; ++a)
        {
            int shiftR = -shapes[s].dr[a];
            int shiftC = col    - shapes[s].dc[a];
            int r[3], c[3];
            bool ok = true;
            for (int i = 0; i < 3; ++i)
            {
                int rr = shapes[s].dr[i] + shiftR;   /* 0..2      */
                int cc = shapes[s].dc[i] + shiftC;   /* 0..4      */
                if (rr < 0 || rr > 2 || cc < 0 || cc > 4) { ok = false; break; }
                if (bits[rr] & (unsigned int)(1u << cc)) { ok = false; break; }
                r[i] = rr; c[i] = cc;
            }
            if (!ok) continue;

            /* Place the tromino.                                   */
            for (int i = 0; i < 3; ++i) bits[r[i]] |= (uint16_t)(1u << c[i]);
            dfs_fill(bits, final);
            for (int i = 0; i < 3; ++i) bits[r[i]] &= (uint16_t)~(1u << c[i]);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Build the full transition table once.                               */
static void build_transitions(void)
{
    seq_t final[STATES];
    uint16_t bits[3];

    for (int cur = 0; cur < 32; ++cur)
    {
        for (int nxt = 0; nxt < 32; ++nxt)
        {
            int state = (cur << 5) | nxt;
            memset(final, 0, sizeof(final));
            bits[0] = (uint16_t)cur;
            bits[1] = (uint16_t)nxt;
            bits[2] = 0;
            dfs_fill(bits, final);

            size_t cnt = 0;
            for (int k = 0; k < STATES; ++k)
            {
                if (final[k] != (seq_t)0)
                {
                    trans[state][cnt].next_cur  = (uint16_t)(k >> 5);
                    trans[state][cnt].next_next = (uint16_t)(k & 31);
                    trans[state][cnt].count     = final[k];
                    ++cnt;
                }
            }
            trans_cnt[state] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/* DP containers and cache.                                            */
static bool   init_done = false;
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  processed_n = 0; /* number of 3‑column blocks processed */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n + 1) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void init(void)
{
    if (init_done) return;
    init_done = true;

    dp_cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    cache   = (seq_t *)calloc(16, sizeof(seq_t));

    dp_cur[0] = (seq_t)1;       /* start with empty state */
    cache[0]  = (seq_t)1;       /* a(0) = 1 */
    processed_n = 0;
    build_transitions();
}

/* ------------------------------------------------------------------ */
/* Public sequence accessor.                                          */
seq_t a(idx_t n)
{
    init();
    ensure_cache(n);

    if (n <= processed_n) return cache[n];

    /* Process 3‑column blocks until we reach n.                      */
    while (processed_n < n)
    {
        /* One column step – apply all transitions.                    */
        for (int step = 0; step < 3; ++step)
        {
            memset(dp_next, 0, STATES * sizeof(seq_t));
            for (int s = 0; s < STATES; ++s)
            {
                seq_t val = dp_cur[s];
                if (!val) continue;
                for (size_t t = 0; t < trans_cnt[s]; ++t)
                {
                    Transition const *tr = &trans[s][t];
                    uint16_t target = (uint16_t)((tr->next_cur << 5) | tr->next_next);
                    /* dp_next[target] += val * tr->count;  */
                    dp_next[target] += val * tr->count;
                }
            }
            /* Prepare for next column.                                 */
            seq_t *swap = dp_cur;
            dp_cur = dp_next;
            dp_next = swap;
        }

        /* One block processed: record result for this n.              */
        ++processed_n;
        cache[processed_n] = dp_cur[0];   /* state (0,0) after block        */
    }
    return cache[n];
}
