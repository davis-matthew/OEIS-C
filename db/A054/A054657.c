/*
 * A054657  Number of leaves on the rooted tree of height n constructed by the
 *          following rule. Assign weight 1 to the single node at height 1. At
 *          each node of weight w at height k>0, branch to nodes at height k+1
 *          as follows: one node of weight 1 and a node of weight d+1 if d
 *          divides w.
 * https://oeis.org/A054657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of leaves at height n in a rooted tree
 * defined by the rule:
 *   start with a single node of weight 1 at height 1.
 *   For each node of weight w at height h>0, create children at
 *   height h+1:
 *       a child of weight 1, and a child of weight d+1
 *           for every positive divisor d of w.
 * The number of nodes at each height satisfies the recurrence:
 *   g_1[1] = 1,
 *   g_{h+1}[1] = Σ_w g_h[w],
 *   g_{h+1}[w] = Σ_{p: (w-1)|p} g_h[p]   (w ≥ 2),
 *   a(h) = Σ_w g_h[w].
 * The implementation below builds the arrays g_h progressively
 * using only integer arithmetic and memoises the results for
 * subsequent calls.  Complexity: O(n²) additions and divisor tests
 * for each n; acceptable for n ≤ 10 000.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static state used for memoisation */
static seq_t *g_prev   = NULL;   /* g_h[w]  for current level h   */
static seq_t *g_next   = NULL;   /* buffer for next level        */
static seq_t *a_vals   = NULL;   /* a(k) values, 1‑indexed       */
static size_t cap      = 0;      /* size of allocated arrays     */
static idx_t  max_done = 0;      /* highest n computed so far    */

/* allocate or grow arrays to hold indices up to n (inclusive) */
static void ensure_arrays(idx_t n)
{
    size_t sz = (size_t)n + 2;          /* indices 0 … n+1 used   */
    if (sz <= cap)
        return;

    seq_t *gp = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *gn = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *av = (seq_t *)calloc(sz, sizeof(seq_t));

    /* copy existing data */
    if (g_prev) memcpy(gp,  g_prev,  cap * sizeof(seq_t));
    if (g_next) memcpy(gn,  g_next,  cap * sizeof(seq_t));
    if (a_vals) memcpy(av,  a_vals,  cap * sizeof(seq_t));

    /* free old storage */
    free(g_prev);
    free(g_next);
    free(a_vals);

    /* bind new arrays */
    g_prev = gp;
    g_next = gn;
    a_vals = av;
    cap    = sz;
}

/* extend precomputation up to and including n */
static void extend_to(idx_t n)
{
    if (n <= max_done)
        return;

    ensure_arrays(n);

    /* initialise height 1 if this is the first call */
    if (max_done == 0) {
        g_prev[1] = 1;
        a_vals[1] = 1;
        max_done  = 1;
    }

    /* iterate from current height +1 up to n */
    for (idx_t level = max_done + 1; level <= n; ++level) {
        /* zero g_next[1..level] before use */
        for (idx_t i = 1; i <= level; ++i)
            g_next[i] = 0;

        seq_t sum_next = 0;

        /* g_next[1] comes from every node at previous level */
        seq_t totalPrev = a_vals[level - 1];
        g_next[1] = totalPrev;
        sum_next += totalPrev;

        /* propagate children from each parent weight p */
        for (idx_t p = 1; p < level; ++p) {
            seq_t val = g_prev[p];
            if (!val)
                continue;

            /* each divisor d of p creates a child of weight d+1 */
            for (idx_t d = 1; d * d <= p; ++d) {
                if (p % d == 0) {
                    idx_t w1 = d + 1;
                    if (w1 <= level) {
                        g_next[w1] += val;
                        sum_next   += val;
                    }
                    idx_t d2 = p / d;
                    if (d2 != d) {
                        idx_t w2 = d2 + 1;
                        if (w2 <= level) {
                            g_next[w2] += val;
                            sum_next   += val;
                        }
                    }
                }
            }
        }

        /* store result for this level */
        a_vals[level] = sum_next;

        /* prepare next iteration */
        seq_t *tmp = g_prev;
        g_prev     = g_next;
        g_next     = tmp;
        max_done   = level;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1 */
    extend_to(n);
    return a_vals[n];
}
