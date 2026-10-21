/*
 * A233855  Number of (n+1)X(3+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 59.
 * https://oeis.org/A233855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A233855
   Number of (n+1)×4 arrays of digits 0..7 whose every 2×2 subblock has
   the sum of the squares of all six edge and diagonal differences equal
   to 59.  The value is computed by a transfer‑matrix dynamic programme
   over rows.  a(1) is the first term shown in the OEIS entry. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/* Representation of a single row: four digits 0..7.                */
/*------------------------------------------------------------------*/
struct Row { uint8_t v[4]; } rows[4096];

/*------------------------------------------------------------------*/
/* Adjacency list for each row.  Each state has a dynamic array of
   indices (uint16_t) of the rows that can follow it.                */
/*------------------------------------------------------------------*/
struct Adj {
    uint16_t *data;  /* pointer to array of neighbours */
    size_t sz;       /* number of stored neighbours   */
    size_t cap;      /* allocated capacity            */
} adj[4096];

/*------------------------------------------------------------------*/
/* Dynamic push into an adjacency list.  Works for both C and C++.   */
/*------------------------------------------------------------------*/
static void adj_push(struct Adj *a, uint16_t v)
{
    if (a->cap == 0) {
        a->cap = 4;
        a->data = (uint16_t*)malloc(a->cap * sizeof(uint16_t));
    } else if (a->sz == a->cap) {
        a->cap <<= 1;
        a->data = (uint16_t*)realloc(a->data, a->cap * sizeof(uint16_t));
    }
    a->data[a->sz++] = v;
}

/*------------------------------------------------------------------*/
/* Test whether two rows can be consecutive in a valid array.      */
/*------------------------------------------------------------------*/
static int ok_pair(const struct Row *r1, const struct Row *r2)
{
    for (int j = 0; j < 3; ++j) {
        int u = r1->v[j];
        int v = r1->v[j + 1];
        int x = r2->v[j];
        int y = r2->v[j + 1];
        int sum = u + v + x + y;
        int T   = u * u + v * v + x * x + y * y;
        if (4 * T - sum * sum != 59) return 0;
    }
    return 1;
}

/*------------------------------------------------------------------*/
/* Initialise all row states and the adjacency graph.               */
/*------------------------------------------------------------------*/
static void init_library(void)
{
    /* Build all 4096 possible rows */
    for (int s = 0; s < 4096; ++s) {
        int x = s;
        rows[s].v[3] = x & 7; x >>= 3;
        rows[s].v[2] = x & 7; x >>= 3;
        rows[s].v[1] = x & 7; x >>= 3;
        rows[s].v[0] = x & 7;
    }

    /* Build adjacency lists */
    for (int i = 0; i < 4096; ++i) {
        struct Adj *ai = &adj[i];
        ai->data = NULL;
        ai->sz   = 0;
        ai->cap  = 0;
        const struct Row *ri = &rows[i];
        for (int j = 0; j < 4096; ++j) {
            if (ok_pair(ri, &rows[j]))
                adj_push(ai, (uint16_t)j);
        }
    }
}

/*------------------------------------------------------------------*/
/* Two static arrays for the DP step (no dynamic allocation).      */
/*------------------------------------------------------------------*/
static seq_t dp_arr1[4096];
static seq_t dp_arr2[4096];

/*------------------------------------------------------------------*/
/* The main function implementing the sequence.                     */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static bool lib_ready = false;
    static idx_t processed = 0;            /* number of n values already computed */
    static seq_t *values = NULL;           /* stored a(1), a(2), ... */
    static size_t values_cap = 0;

    static seq_t *dp_cur = dp_arr1;
    static seq_t *dp_next = dp_arr2;

    if (!lib_ready) {
        init_library();                      /* build the adjacency graph */

        /* initialise dp_cur to all 1s (each first row is a solution) */
        for (int i = 0; i < 4096; ++i)
            dp_cur[i] = (seq_t)1;

        values_cap = 16;
        values = (seq_t*)malloc(values_cap * sizeof(seq_t));
        processed = 0;
        lib_ready = true;
    }

    /* compute values up to requested n */
    while (processed < n) {
        /* zero dp_next */
        for (int i = 0; i < 4096; ++i)
            dp_next[i] = (seq_t)0;

        /* transition to next row */
        for (int i = 0; i < 4096; ++i) {
            seq_t val = dp_cur[i];
            if ((seq_t)0 == val) continue;
            const uint16_t *list = adj[i].data;
            size_t sz = adj[i].sz;
            for (size_t k = 0; k < sz; ++k)
                dp_next[list[k]] += val;
        }

        /* sum to obtain a(processed+1) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 4096; ++i)
            total += dp_next[i];

        /* store the result */
        if (values_cap <= processed) {
            size_t new_cap = values_cap ? values_cap * 2 : 16;
            while (new_cap <= processed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
            values = tmp;
            values_cap = new_cap;
        }
        values[processed] = total;

        /* prepare for next iteration: copy dp_next to dp_cur */
        for (int i = 0; i < 4096; ++i)
            dp_cur[i] = dp_next[i];

        ++processed;
    }

    return values[n - 1];
}
