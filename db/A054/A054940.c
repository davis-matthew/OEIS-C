/*
 * A054940  Number of connected labeled graphs with n nodes and an odd number
 *          of edges.
 * https://oeis.org/A054940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A001187(n)+(-1)^n*A000142(n-1))/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial.h"   /* factorial(idx_t) -> seq_t */
#include "../utils/pow2.h"        /* pow2(idx_t) -> seq_t */
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) -> seq_t */

static seq_t *conn_cache = NULL;      /* cache for A001187 values, 1‑based */
static idx_t  conn_cap   = 0;
static idx_t  conn_max   = 0;

/* Compute the number of connected labeled graphs on m vertices (A001187) */
static seq_t
connected_graphs(idx_t n)
{
    /* Lazy initialization of the cache */
    if (conn_cache == NULL) {
        conn_cap = 16;
        conn_cache = (seq_t *)malloc(conn_cap * sizeof(seq_t));
        if (!conn_cache) return 0;            /* allocation failure – should not happen */
        conn_cache[0] = 0;                    /* unused */
        conn_cache[1] = 1;                    /* one graph on a single vertex */
        conn_max   = 1;
    }

    /* Extend the cache up to n */
    while (conn_max < n) {
        idx_t m = conn_max + 1;

        /* total number of graphs on m vertices */
        idx_t exp_total = (m * (m - 1)) / 2;
        seq_t total = pow2(exp_total);

        /* subtract disconnected graphs */
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t bin = binomial(m - 1, k - 1);               /* C(m-1, k-1) */
            idx_t exp_part = ((m - k) * (m - k - 1)) / 2;
            seq_t part = pow2(exp_part);                       /* 2^((m-k)*(m-k-1)/2) */
            seq_t term = bin * conn_cache[k] * part;
            sum += term;
        }

        seq_t conn = total - sum;

        /* Grow cache if necessary (doubling strategy) */
        if (m >= conn_cap) {
            idx_t new_cap = conn_cap * 2;
            seq_t *tmp = (seq_t *)realloc(conn_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure – should not happen */
            conn_cache = tmp;
            conn_cap   = new_cap;
        }

        conn_cache[m] = conn;
        conn_max = m;
    }
    return conn_cache[n];
}

/* OEIS A054940: connected labeled graphs with n nodes and an odd number of edges */
seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the specification */
    seq_t conn = connected_graphs(n);          /* A001187(n) */
    seq_t fact = factorial(n - 1);            /* (n‑1)! = A000142(n-1) */

    /* Add or subtract fact depending on the parity of n */
    if ((n & 1) == 0)            /* n even → add (n‑1)!  */
        conn += fact;
    else                         /* n odd  → subtract (n‑1)! */
        conn -= fact;

    /* Exact division by 2 – the result is always an integer */
    conn /= 2;
    return conn;
}
