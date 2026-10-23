/*
 * A194543  Triangle T(n,k), n>=0, 0<=k<=n, read by rows: T(n,k) is the number
 *          of partitions of n into parts p_i such that |p_i - p_j| >= k for i
 *          != j.
 * https://oeis.org/A194543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = largest integer such that (r+1)(r+2)/2
 *            <= n+1 * k = n - r(r+1)/2 * T(r,k) is the number of partitions
 *            of r in which the difference between * any two parts is at least
 *            k. * * The function precomputes T(r,k) on demand for all rows up
 *            to the largest * r needed. The recurren...
 * Range:     u8 n <= 152, u16 n <= 989, u32 n <= 8255, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t * *T_table      = NULL;   /* triangle storage, rows [0..] */
static size_t   T_rows_alloc = 0;      /* number of rows allocated */

/* --------------------------------------------------------------- */
/*  Memoised recursion for a fixed (r,k).  */
/* --------------------------------------------------------------- */
static size_t   g_n  = 0;                  /* current n for recursion */
static size_t   g_k  = 0;                  /* current k for recursion */
static seq_t   *g_memo = NULL;             /* memoisation table */
static unsigned char *g_vis = NULL;        /* visited flags */
static size_t   g_memo_cap = 0;            /* capacity of memo table */

/* Allocate or resize the memo tables to hold (g_n+1)*(g_n+1) entries. */
static void init_memo(size_t sz)
{
    if (g_memo_cap < sz)
    {
        if (g_memo) free(g_memo);
        if (g_vis) free(g_vis);
        g_memo = (seq_t*)malloc(sz * sizeof(seq_t));
        g_vis  = (unsigned char*)malloc(sz * sizeof(unsigned char));
        g_memo_cap = sz;
    }
    /* clear visited flags for the new size */
    memset(g_vis, 0, sz * sizeof(unsigned char));
}

/* Recursive helper.  Assumes g_n, g_k, g_memo, g_vis set. */
static seq_t rec_fun(size_t rem, size_t last)
{
    if (rem == 0) return 1;          /* empty remainder is one partition */
    if (last == 0) return 0;          /* cannot choose any further part */

    size_t idx = rem * (g_n + 1) + last;
    if (g_vis[idx]) return g_memo[idx];

    seq_t sum = 0;
    size_t limit = (last < rem) ? last : rem;
    for (size_t x = 1; x <= limit; ++x)
    {
        size_t next_last = (g_k == 0) ? x :
                           (x >= g_k ? x - g_k : 0);
        sum += rec_fun(rem - x, next_last);
    }
    g_vis[idx] = 1;
    g_memo[idx] = sum;
    return sum;
}

/* Compute T(n,k) by recursive memoisation. */
static seq_t compute_T_arbitrary(idx_t nn, idx_t kk)
{
    g_n = (size_t)nn;
    g_k = (size_t)kk;
    size_t sz = (g_n + 1) * (g_n + 1);
    init_memo(sz);
    return rec_fun(g_n, g_n);
}

/* --------------------------------------------------------------- */
/*  Triangle storage helpers.  */
/* --------------------------------------------------------------- */
static void ensure_rows_allocated(size_t needed)
{
    if (needed < T_rows_alloc) return;
    size_t new_rows = needed + 1;
    seq_t **new_table = (seq_t**)realloc(T_table, new_rows * sizeof(seq_t*));
    T_table = new_table;
    for (size_t i = T_rows_alloc; i < new_rows; ++i)
    {
        T_table[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));  /* zero init */
    }
    T_rows_alloc = new_rows;
}

/* Compute all values of row r that have not yet been computed. */
static void compute_row_uncomputed(size_t r)
{
    seq_t *row = T_table[r];
    for (size_t k = 0; k <= r; ++k)
    {
        if (row[k] == 0)                /* zero means uncomputed */
            row[k] = compute_T_arbitrary((idx_t)r, (idx_t)k);
    }
}

/* --------------------------------------------------------------- */
/*  Public sequence function.  */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine row r and column k from index n (OEIS triangle read by rows). */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    size_t prev_cum = r * (r + 1) / 2;  /* indices before row r */
    size_t k = n - prev_cum;

    /* Allocate triangle storage up to row r. */
    ensure_rows_allocated(r);

    /* If the row has not yet been filled, compute it. */
    if (T_table[r][0] == 0)
        compute_row_uncomputed(r);

    return T_table[r][k];
}
