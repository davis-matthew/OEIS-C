/*
 * A205039  Number of (n+1)X2 0..6 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A205039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays with entries 0..6 whose
 *            permanents of all 2×2 subblocks are equal and nonzero. The code
 *            builds a directed graph for each possible permanent value and
 *            counts length‑n+1 sequences via dynamic programming. It caches
 *            results up to the largest n requested, so any order...
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_V 72      /* maximum permanent value */
#define ROWS 49       /* 0..6 × 0..6 rows */

typedef struct {
    idx_t neigh[7];   /* up to 7 outgoing edges per row */
    unsigned char count;
} EdgeList;

static EdgeList edges[MAX_V + 1][ROWS];
static seq_t cur[MAX_V + 1][ROWS];
static seq_t next[MAX_V + 1][ROWS];
static seq_t totals[MAX_V + 1];
static seq_t *ans = NULL;          /* cached a(n) values */
static idx_t ans_size = 0;
static idx_t steps_done = 0;       /* number of transitions already computed */

/* Build adjacency lists once */
static void init_globals(void)
{
    static int built = 0;
    if (built) return;
    built = 1;

    /* precompute row values */
    int a_vals[ROWS], b_vals[ROWS];
    for (int a = 0; a <= 6; ++a)
        for (int b = 0; b <= 6; ++b)
        {
            int idx = a * 7 + b;
            a_vals[idx] = a;
            b_vals[idx] = b;
        }

    /* build edges for each permanent value */
    for (int v = 1; v <= MAX_V; ++v)
        for (int i = 0; i < ROWS; ++i)
            edges[v][i].count = 0;

    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
        {
            int p = a_vals[i] * b_vals[j] + b_vals[i] * a_vals[j];
            if (p >= 1 && p <= MAX_V)
            {
                EdgeList *el = &edges[p][i];
                el->neigh[el->count++] = (idx_t)j;
            }
        }

    /* initialize dynamic programming tables */
    for (int v = 1; v <= MAX_V; ++v)
        for (int i = 0; i < ROWS; ++i)
            cur[v][i] = (seq_t)1;     /* one way to start at row i */

    for (int v = 1; v <= MAX_V; ++v)
        totals[v] = (seq_t)ROWS;      /* sum of cur */

    /* allocate answer cache, index 0 unused */
    ans_size = 2;                    /* will store ans[0] and ans[1] after first step */
    ans = (seq_t *)malloc(ans_size * sizeof(seq_t));
    if (!ans) abort();               /* fatal error if allocation fails */
    ans[0] = (seq_t)0;
    ans[1] = (seq_t)0;               /* will be computed lazily */
}

/* Ensure cached answers up to n are available */
static void ensure_ans(idx_t n)
{
    init_globals();

    /* enlarge answer cache if necessary */
    if (n + 1 >= ans_size)
    {
        size_t new_size = (size_t)n + 2;
        seq_t *new_ans = (seq_t *)realloc(ans, new_size * sizeof(seq_t));
        if (!new_ans) abort();
        ans = new_ans;
        for (size_t i = ans_size; i < new_size; ++i)
            ans[i] = (seq_t)0;
        ans_size = new_size;
    }

    /* compute transitions until we have step n */
    while (steps_done < n)
    {
        /* clear next table */
        for (int v = 1; v <= MAX_V; ++v)
            for (int i = 0; i < ROWS; ++i)
                next[v][i] = (seq_t)0;

        /* propagate one step */
        for (int v = 1; v <= MAX_V; ++v)
        {
            for (int i = 0; i < ROWS; ++i)
            {
                seq_t val = cur[v][i];
                if (val == (seq_t)0) continue;
                EdgeList *el = &edges[v][i];
                for (int k = 0; k < el->count; ++k)
                {
                    int j = el->neigh[k];
                    next[v][j] += val;
                }
            }
        }

        /* compute totals for each permanent value */
        seq_t sum_totals = (seq_t)0;
        for (int v = 1; v <= MAX_V; ++v)
        {
            seq_t total = (seq_t)0;
            for (int j = 0; j < ROWS; ++j)
                total += next[v][j];
            totals[v] = total;
            sum_totals += total;
        }

        /* store answer for this step count */
        ans[steps_done + 1] = sum_totals;

        /* copy next to cur for next iteration */
        for (int v = 1; v <= MAX_V; ++v)
            for (int i = 0; i < ROWS; ++i)
                cur[v][i] = next[v][i];

        ++steps_done;
    }
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is defined for n>=1 only */
    ensure_ans(n);
    return ans[(size_t)n];
}
