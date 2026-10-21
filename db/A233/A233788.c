/*
 * A233788  Number of (n+1)X(4+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A233788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233788 – Number of (n+1)×5 arrays with entries 0..3 in which every
 * 2×2 subblock has the sum of the squares of all six edge and diagonal
 * differences equal to 11.
 *
 * The sequence is computed by a transfer‑matrix/backtracking algorithm.
 * Each row of length five is represented by a state (0…1023).  All
 * 4⁵ = 1024 states are pre‑computed, together with a table of which
 * pairs of consecutive states are compatible (i.e. all four overlapping
 * 2×2 blocks meet the sum‑of‑squares = 11 condition).  The number of
 * arrays with (n+1) rows is obtained by iterating n times through the
 * adjacency graph; the result for index n is the sum of the counts
 * after the nth transition.  Memoisation guarantees that each n is
 * computed at most once.
 *
 * Function:
 *   seq_t a(idx_t n);
 *   (Return 0 for n == 0 – out of range.)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Configuration */
/* Number of possible rows: 4^5 = 1024 */
#define STATES 1024

/* Square of difference values 0..3 */
static const int DIFF_SQ[4] = {0, 1, 4, 9};

/* Digit of a state s at position p (0 = leftmost) */
static unsigned char state_digit[STATES][5];

/* Adjacency lists */
static int *succ_start = NULL; /* [STATES+1] */
static int *succ_count = NULL; /* [STATES]   */
static int *succ_edges = NULL; /* [total_edges] */

/* DP buffers */
static seq_t *dp_cur = NULL;  /* counts for current height */
static seq_t *dp_nxt = NULL;  /* temporary buffer */

/* Memoised cache of a[1..max_cached] */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static idx_t  max_cached = 0; /* largest n stored in cache */

/* Current height represented by dp_cur */
static idx_t cur_rows = 1;

/* --------------------------------------------------------------------- */
/* Build integer → 5‑digit state mapping. */
static void build_state_mapping(void)
{
    for (int s = 0; s < STATES; ++s) {
        int x = s;
        /* positions 4..0 (right to left) */
        for (int pos = 4; pos >= 0; --pos) {
            state_digit[s][pos] = (unsigned char)(x & 3);
            x >>= 2;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Check compatibility of two consecutive states. */
static bool valid_transition(int i, int j)
{
    const unsigned char *si = state_digit[i];
    const unsigned char *sj = state_digit[j];

    for (int c = 0; c < 4; ++c) {
        int a  = si[c];
        int b  = si[c + 1];
        int c1 = sj[c];
        int d  = sj[c + 1];

        int sum =  DIFF_SQ[ abs(a - b) ] +
                   DIFF_SQ[ abs(a - c1) ] +
                   DIFF_SQ[ abs(b - d) ] +
                   DIFF_SQ[ abs(c1 - d) ] +
                   DIFF_SQ[ abs(a - d) ] +
                   DIFF_SQ[ abs(b - c1) ];

        if (sum != 11)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Build adjacency lists. */
static void build_adjacency(void)
{
    succ_start = (int *)malloc((STATES + 1) * sizeof(int));
    succ_count = (int *)malloc(STATES * sizeof(int));
    for (int i = 0; i < STATES; ++i)
        succ_count[i] = 0;

    /* Count edges for each source state. */
    for (int i = 0; i < STATES; ++i) {
        for (int j = 0; j < STATES; ++j)
            if (valid_transition(i, j))
                ++succ_count[i];
    }

    /* Prefix sums to obtain start indices. */
    succ_start[0] = 0;
    for (int i = 0; i < STATES; ++i)
        succ_start[i + 1] = succ_start[i] + succ_count[i];

    int total_edges = succ_start[STATES];
    succ_edges = (int *)malloc(total_edges * sizeof(int));

    /* Fill edges. */
    for (int i = 0; i < STATES; ++i) {
        int pos = succ_start[i];
        for (int j = 0; j < STATES; ++j)
            if (valid_transition(i, j))
                succ_edges[pos++] = j;
    }
}

/* --------------------------------------------------------------------- */
/* Initialise everything on first call. */
static void initialise(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    build_state_mapping();
    build_adjacency();

    dp_cur = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_nxt = (seq_t *)calloc(STATES, sizeof(seq_t));
    for (int i = 0; i < STATES; ++i)
        dp_cur[i] = (seq_t)1;  /* single‑row arrays */

    cache     = NULL;
    cache_cap = 0;
    max_cached = 0;
    cur_rows  = 1;
}

/* --------------------------------------------------------------------- */
/* Ensure cache can store indices up to 'needed'. */
static void cache_grow(idx_t needed)
{
    if ((size_t)needed + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)needed + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    /* Initialise new entries to zero */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range */

    initialise();

    /* Compute missing terms up to n. */
    while (cur_rows <= n) {
        /* Reset dp_nxt for the next transition. */
        memset(dp_nxt, 0, STATES * sizeof(seq_t));

        /* Transition dp_cur -> dp_nxt. */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0)
                continue;
            int start = succ_start[i];
            int cnt   = succ_count[i];
            for (int k = 0; k < cnt; ++k) {
                int dest = succ_edges[start + k];
                dp_nxt[dest] += val;
            }
        }

        /* Sum counts for arrays of height cur_rows+1. */
        seq_t total = (seq_t)0;
        for (int j = 0; j < STATES; ++j)
            total += dp_nxt[j];

        /* Store result for index cur_rows (i.e. a(cur_rows)). */
        cache_grow(cur_rows);
        cache[cur_rows] = total;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;

        ++cur_rows;        /* now represent height cur_rows+1 */
        ++max_cached;      /* a cur_rows just stored */
    }

    return cache[n];
}
