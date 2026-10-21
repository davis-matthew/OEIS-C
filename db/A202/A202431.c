/*
 * A202431  Number of (n+2)X6 binary arrays avoiding patterns 000 and 001 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202431
 *  Number of (n+2)×6 binary arrays avoiding patterns 000 and 001
 *  in rows, columns and NW‑to‑SE diagonals.
 *
 *  The array is computed with a transfer‑matrix DP.
 *  States are ordered pairs of valid 6‑bit rows.
 *  A transition from (r1,r2) to (r2,r3) is allowed iff
 *      – each of r1,r2,r3 satisfies the horizontal restriction
 *      – for every column j:  (r1[j],r2[j],r3[j]) ≠ (0,0,0),(0,0,1)
 *      – for every diagonal j≥2: (r1[j-2],r2[j-1],r3[j]) ≠ (0,0,0),(0,0,1)
 *
 *  The initial state contains all 24 valid rows.  The DP starts from
 *  the first two rows (all pairs weighted 1), then each step adds a
 *  new row.  After k steps we have counted arrays of k+2 rows, i.e.
 *  a(k).  The function memoises all previously computed terms.
 */
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int init_done = 0;               /* has the static data been built? */

/* number of valid 6‑bit rows */
static size_t Rcnt = 0;
/* mask of each valid row, indexed 0..Rcnt-1 */
static int *row_mask = NULL;
/* mapping from mask (0..63) to index or -1 if invalid */
static int mask_index[64] = {0};

/* number of states = Rcnt×Rcnt */
static size_t S = 0;
/* adjacency list: for each state s, next_indices[s*60 + k]
 * (0 ≤ k < deg[s]) holds destination state index.
 * The maximum possible degree is Rcnt ≤ 64. */
static int *next_indices = NULL;          /* allocated S×64 int */
static unsigned char *deg = NULL;          /* allocated S unsigned char */

/* dynamic programming containers */
static seq_t *dp_cur = NULL;            /* current state vector */
static seq_t *dp_next = NULL;           /* temporary vector */

/* memoised sequence values */
static seq_t *sum_cache = NULL;         /* index n → a(n) */
static size_t sum_cap = 0;              /* allocated capacity for sum_cache */
static size_t max_cached = 0;           /* largest n already computed */

/* ------------------------------------------------------------------ */
/* build static data: valid rows, adjacency list, DP containers     */
/* ------------------------------------------------------------------ */
static void init_state(void)
{
    if (init_done)
        return;

    /* ---------- find all valid rows (no 000 or 001 horizontally) --------- */
    for (int m = 0; m < 64; ++m)
        mask_index[m] = -1;
    for (int m = 0; m < 64; ++m) {
        int ok = 1;
        for (int j = 0; j < 4 && ok; ++j) {
            int b0 = (m >> (5 - j)) & 1;
            int b1 = (m >> (5 - (j + 1))) & 1;
            int b2 = (m >> (5 - (j + 2))) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 0 && b1 == 0 && b2 == 1))
                ok = 0;
        }
        if (ok) {
            mask_index[m] = (int)Rcnt;
            ++Rcnt;
        }
    }

    /* ---------- store masks of valid rows ---------- */
    row_mask = (int *)calloc(Rcnt, sizeof(int));
    for (int m = 0; m < 64; ++m) {
        if (mask_index[m] >= 0)
            row_mask[mask_index[m]] = m;
    }

    /* ---------- number of states ---------- */
    S = Rcnt * Rcnt;

    /* ---------- adjacency list ---------- */
    next_indices = (int *)calloc(S * 64, sizeof(int)); /* 64 is upper bound per state */
    deg = (unsigned char *)calloc(S, sizeof(unsigned char));

    /* helper macro to extract a bit at position j (0 leftmost, 5 rightmost) */
#define BIT(mask, j)  (((mask) >> (5 - (j))) & 1)

    for (size_t a = 0; a < Rcnt; ++a) {
        int a_mask = row_mask[a];
        for (size_t b = 0; b < Rcnt; ++b) {
            int b_mask = row_mask[b];
            size_t s = a * Rcnt + b;
            unsigned char d = 0;

            for (size_t c = 0; c < Rcnt; ++c) {
                int c_mask = row_mask[c];
                int ok = 1;

                /* vertical triples */
                for (int j = 0; j < 6 && ok; ++j) {
                    int ba = BIT(a_mask, j);
                    int bb = BIT(b_mask, j);
                    int bc = BIT(c_mask, j);
                    if ((ba == 0 && bb == 0 && bc == 0) ||
                        (ba == 0 && bb == 0 && bc == 1))
                        ok = 0;
                }

                /* diagonal NW–SE triples */
                for (int j = 2; j < 6 && ok; ++j) {
                    int ba = BIT(a_mask, j - 2);
                    int bb = BIT(b_mask, j - 1);
                    int bc = BIT(c_mask, j);
                    if ((ba == 0 && bb == 0 && bc == 0) ||
                        (ba == 0 && bb == 0 && bc == 1))
                        ok = 0;
                }

                if (ok) {
                    next_indices[s * 64 + d] = b * Rcnt + c;
                    ++d;
                }
            }
            deg[s] = d;
        }
    }

    /* ---------- allocate DP vectors ---------- */
    dp_cur = (seq_t *)calloc(S, sizeof(seq_t));
    dp_next = (seq_t *)calloc(S, sizeof(seq_t));
    for (size_t s = 0; s < S; ++s)     /* all pairs of first two rows */
        dp_cur[s] = (seq_t)1;

    /* ---------- allocate memoisation storage ---------- */
    sum_cap = 32;                      /* initial capacity */
    sum_cache = (seq_t *)calloc(sum_cap + 1, sizeof(seq_t)); /* index 0 unused */

    max_cached = 0;                     /* no terms computed yet */

    init_done = 1;
#undef BIT
}

/* ------------------------------------------------------------------ */
/* public sequence function                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_state();

    /* return already computed result, if any */
    if (n <= max_cached)
        return sum_cache[n];

    /* ensure memoisation array large enough */
    if (n > sum_cap) {
        size_t new_cap = sum_cap * 2;
        if (new_cap < n)
            new_cap = n;
        sum_cache = (seq_t *)realloc(sum_cache, (new_cap + 1) * sizeof(seq_t));
        if (!sum_cache) return (seq_t)0;  /* out‑of‑memory, unlikely */
        for (size_t i = sum_cap + 1; i <= new_cap; ++i)
            sum_cache[i] = (seq_t)0;
        sum_cap = new_cap;
    }

    /* iterative DP from max_cached+1 up to n */
    for (idx_t cur_n = max_cached + 1; cur_n <= n; ++cur_n) {
        /* zero next vector */
        memset(dp_next, 0, sizeof(seq_t) * S);

        /* transition */
        for (size_t s = 0; s < S; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == (seq_t)0)
                continue;
            size_t base = s * 64;
            unsigned char d = deg[s];
            for (unsigned char k = 0; k < d; ++k) {
                size_t t = (size_t)next_indices[base + k];
                dp_next[t] += cnt;
            }
        }

        /* compute total */
        seq_t total = (seq_t)0;
        for (size_t v = 0; v < S; ++v)
            total += dp_next[v];

        sum_cache[cur_n] = total;

        /* swap vectors for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    max_cached = n;
    return sum_cache[n];
}
