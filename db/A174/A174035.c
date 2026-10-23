/*
 * A174035  Triangle read by rows: T(n,m) = binomial(n, m) + floor(Eulerian(n
 *          + 1, m)/2).
 * https://oeis.org/A174035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A174035: Flattened triangle T(n,m) = binomial(n,m) + floor(Eulerian(n+1,m)/2)
   a(n) is the term at index n in the row‑by‑row listing of the triangle.
   The function computes a(n) for any n ≥ 0 (non‑negative index) using only integer
   arithmetic.  It memoises Eulerian numbers to avoid recomputation. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

static seq_t **eul_rows   = NULL;   /* eul_rows[i]  → array of Eulerian(i,k), k=0..i */
static idx_t  eul_max     = 0;      /* highest row computed (0 based) */

/* Ensure Eulerian numbers are computed for all rows up to 'max_needed' (inclusive). */
static void
ensure_euler(idx_t max_needed)
{
    if (max_needed <= eul_max)
        return;

    /* Resize the array of row pointers. */
    seq_t **new_rows = (seq_t **)realloc(eul_rows, (size_t)(max_needed + 1) * sizeof(seq_t *));
    if (!new_rows) abort();          /* memory allocation failure */
    eul_rows = new_rows;

    /* Compute new rows. */
    for (idx_t i = eul_max + 1; i <= max_needed; ++i) {
        /* Each row i has i+1 elements (indices 0..i). */
        seq_t *row = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!row) abort();

        if (i == 0) {                      /* A(0,0)=1 */
            row[0] = (seq_t)1;
        } else if (i == 1) {                /* A(1,0)=1 */
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = eul_rows[i - 1];
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k > 0)      /* (i - k) * prev[k-1] */
                    val += (seq_t)(i - k) * prev[k - 1];
                if (k < i)      /* (k + 1) * prev[k] */
                    val += (seq_t)(k + 1) * prev[k];
                row[k] = val;
            }
        }
        eul_rows[i] = row;
    }
    eul_max = max_needed;
}

seq_t a(idx_t n)
{
    /* Determine the row r such that the (r+1)th row starts at index
       C(r-1) = r*(r+1)/2 and ends before C(r) = (r+1)*(r+2)/2. */
    idx_t r;
    if (n == 0) {
        r = 0;
    } else {
        /* Use integer arithmetic: r = floor( (sqrt(8n+1)-1)/2 ). */
        long double val = sqrtl((long double)8.0L * (long double)n + 1.0L);
        r = (idx_t)((val - 1.0L) / 2.0L);
    }

    /* Column index m within that row. */
    idx_t m = n - r * (r + 1) / 2;   /* r*(r+1)/2 = cumulative count up to row r-1 */

    /* Ensure Eulerian numbers for row r+1 (needed for Eulerian(r+1,m)). */
    ensure_euler(r + 1);

    seq_t bin = binomial(r, m);                 /* binomial(r,m) */
    seq_t eul = eul_rows[r + 1][m];             /* Eulerian(r+1,m) */
    return bin + eul / (seq_t)2;                /* floor division by 2 */
}
