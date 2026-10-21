/*
 * A207407  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via dynamic programming for 7×n binary matrices
 *            avoiding 0 0 1 and 0 1 1 horizontally and 0 0 1 and 1 1 0
 *            vertically
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int Vlist[128];           /* valid 7‑bit columns */
static int Vsize;                /* number of valid columns */
static size_t numStates;         /* Vsize * Vsize */

/* Transition data: flattened adjacency lists for states (p,q) → (q,r)   */
static int *trans_start;         /* start index in trans_all */
static int *trans_len;           /* number of successors per state */
static int *trans_all;           /* all target states */

/* DP buffers: counts for current and next column lengths               */
static seq_t *dp_cur;
static seq_t *dp_next;

/* Cache of previously computed terms                                   */
static seq_t *term_cache;
static size_t term_cache_size;
static size_t term_cache_cap;

/* Initialization flag                                                  */
static bool initialized = false;

static void init(void)
{
    if (initialized) return;

    /* 1. Find all 7‑bit columns that avoid the vertical patterns.     */
    Vsize = 0;
    for (int col = 0; col < 128; ++col) {
        bool ok = true;
        for (int i = 0; i + 2 < 7 && ok; ++i) {
            int a = (col >> i)       & 1;
            int b = (col >> (i+1))   & 1;
            int c = (col >> (i+2))   & 1;
            if ((a == 0 && b == 0 && c == 1) ||
                (a == 1 && b == 1 && c == 0))
                ok = false;
        }
        if (ok) Vlist[Vsize++] = col;
    }

    numStates = (size_t)Vsize * Vsize;

    /* 2. Build adjacency lists for transitions based on horizontal rule. */
    trans_start = (int*)malloc(numStates * sizeof(int));
    trans_len   = (int*)malloc(numStates * sizeof(int));

    size_t totalEdges = 0;
    for (int p = 0; p < Vsize; ++p) {
        for (int q = 0; q < Vsize; ++q) {
            int idx = p * Vsize + q;
            int mask = (~Vlist[p]) & 0x7F;   /* bits that must be 0 in r   */
            int cnt = 0;
            for (int r = 0; r < Vsize; ++r)
                if ((Vlist[r] & mask) == 0) ++cnt;
            trans_len[idx] = cnt;
            totalEdges += cnt;
        }
    }

    int pos = 0;
    for (int i = 0; i < (int)numStates; ++i) {
        trans_start[i] = pos;
        pos += trans_len[i];
    }

    trans_all = (int*)malloc(totalEdges * sizeof(int));
    int *curr = (int*)malloc(numStates * sizeof(int));
    for (int i = 0; i < (int)numStates; ++i) curr[i] = trans_start[i];

    for (int p = 0; p < Vsize; ++p) {
        for (int q = 0; q < Vsize; ++q) {
            int idx = p * Vsize + q;
            int mask = (~Vlist[p]) & 0x7F;
            for (int r = 0; r < Vsize; ++r)
                if ((Vlist[r] & mask) == 0)
                    trans_all[ curr[idx]++ ] = q * Vsize + r; /* next state (q,r) */
        }
    }
    free(curr);

    /* 3. Allocate DP buffers. */
    dp_cur  = (seq_t*)calloc(numStates, sizeof(seq_t));
    dp_next = (seq_t*)calloc(numStates, sizeof(seq_t));

    /* 4. Allocate cache for sequence terms. */
    term_cache_cap = 256;
    term_cache_size = 0;
    term_cache = (seq_t*)malloc(term_cache_cap * sizeof(seq_t));

    initialized = true;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n == 0) return 0;

    init();                                   /* one‑time setup */

    /* Ensure first two terms are available. */
    if (term_cache_size < 2) {
        /* a(1) = number of valid columns */
        term_cache[0] = (seq_t)Vsize;
        /* a(2) = Vsize^2 (all pairs of valid columns) */
        term_cache[1] = (seq_t)Vsize * (seq_t)Vsize;
        term_cache_size = 2;

        /* Initialise DP for n = 2: one way for each pair of columns. */
        for (size_t i = 0; i < numStates; ++i)
            dp_cur[i] = (seq_t)1;
    }

    /* Extend cache up to requested n. */
    while (term_cache_size < (size_t)n) {
        /* Clear dp_next buffer */
        memset(dp_next, 0, numStates * sizeof(seq_t));

        /* Perform transitions from dp_cur to dp_next */
        for (size_t s = 0; s < numStates; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;

            int start = trans_start[s];
            int len   = trans_len[s];
            for (int k = 0; k < len; ++k) {
                int tgt = trans_all[start + k];
                dp_next[tgt] += val;
            }
        }

        /* Sum all counts for the next column length to obtain a(k+1). */
        seq_t sum = 0;
        for (size_t i = 0; i < numStates; ++i)
            sum += dp_next[i];

        /* Store the newly computed term. */
        if (term_cache_size >= term_cache_cap) {
            term_cache_cap *= 2;
            term_cache = (seq_t*)realloc(term_cache,
                                         term_cache_cap * sizeof(seq_t));
        }
        term_cache[term_cache_size++] = sum;

        /* Prepare for next iteration. */
        seq_t *tmp   = dp_cur;
        dp_cur       = dp_next;
        dp_next      = tmp;
    }

    return term_cache[n - 1];
}
