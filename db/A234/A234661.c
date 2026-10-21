/*
 * A234661  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   _row_compatible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A234661:
   a(n) is the number of (n+1)×4 arrays with elements 0..6 such that
   every 2×2 subblock has the property that the sum of its main
   diagonal differs from the sum of its anti‑diagonal by exactly 4.
   The implementation builds a finite state automaton on the 7⁴=2401
   possible rows, precomputes a table of compatible successor rows,
   and performs a dynamic‑programming walk of length n.
   The result is computed in O(2401·deg·n) time with a very small
   constant factor. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_row_compatible.h"

/* Parameters ----------------------------------------------------- */
#define ROWS 2401          /* 7⁴ possible rows */
/* We use base 7 digits 0..6 */
#define BASE 7

/* Static data ---------------------------------------------------- */
static bool          _instantiated = false;
static int           rows[ROWS][4];          /* row contents      */
static int          *adj;                   /* adjacency list     */
static size_t       *adj_start;             /* start index in adj */
static size_t        adj_len[ROWS];         /* list lengths       */
static seq_t        dp0[ROWS], dp1[ROWS], *dp_cur, *dp_next;

/* Build the automaton ------------------------------------------- */
static void _init_automaton(void)
{
    /* Enumerate all rows */
    for (int i = 0; i < ROWS; ++i) {
        int x = i;
        rows[i][0] =  x % BASE; x /= BASE;
        rows[i][1] =  x % BASE; x /= BASE;
        rows[i][2] =  x % BASE; x /= BASE;
        rows[i][3] =  x % BASE;
    }

    /* Count neighbours to preallocate adjacency storage */
    size_t total = 0;
    for (int i = 0; i < ROWS; ++i) {
        size_t cnt = 0;
        for (int j = 0; j < ROWS; ++j)
            if (_row_compatible(rows[i], rows[j])) ++cnt;
        adj_len[i] = cnt;
        total += cnt;
    }

    /* Allocate adjacency arrays (C++ cast needed) */
    adj = (int*)malloc(total * sizeof(int));
    adj_start = (size_t*)malloc((ROWS + 1) * sizeof(size_t));
    adj_start[0] = 0;
    for (int i = 0; i < ROWS; ++i)
        adj_start[i + 1] = adj_start[i] + adj_len[i];

    /* Fill adjacency lists */
    for (int i = 0; i < ROWS; ++i) {
        size_t pos = adj_start[i];
        for (int j = 0; j < ROWS; ++j)
            if (_row_compatible(rows[i], rows[j]))
                adj[pos++] = j;
    }

    dp_cur = dp0;
    dp_next = dp1;

    _instantiated = true;
}

/* Sequence function ------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (!_instantiated)
        _init_automaton();

    /* a(1) corresponds to n == 1 (two rows); a(0) is unused */
    if (n == 0)
        return 0;

    /* initial DP: one way for each first row */
    for (size_t i = 0; i < ROWS; ++i)
        dp_cur[i] = 1;

    /* walk n steps (each adds one more row) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next DP */
        for (size_t i = 0; i < ROWS; ++i)
            dp_next[i] = 0;

        /* transitions */
        for (size_t i = 0; i < ROWS; ++i) {
            seq_t v = dp_cur[i];
            if (!v) continue;
            size_t start = adj_start[i];
            size_t end   = adj_start[i + 1];
            for (size_t p = start; p < end; ++p)
                dp_next[adj[p]] += v;
        }

        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states */
    seq_t result = 0;
    for (size_t i = 0; i < ROWS; ++i)
        result += dp_cur[i];

    return result;
}
