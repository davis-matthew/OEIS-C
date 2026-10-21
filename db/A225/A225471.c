/*
 * A225471  Triangle read by rows, s_4(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225471 (Stirling‑Frobenius 4th‑order cycle numbers read by rows).
   The triangle entries are produced by the recurrence for the row
   polynomials R(n,x) = (x+3) * R(n-1,x+4), with R(0,x)=1.
   The sequence a(n) lists the triangle left‑to‑right, row by row.
   The implementation builds the triangle lazily until the requested
   index is reached.  All arithmetic is performed with type seq_t,
   which may be an arbitrary‑precision integer or a fixed‑width unsigned
   integer.  The algorithm is O(r²) per new row where r≈√(2n).  */
 
#include <stdlib.h>
#include <stdint.h>

/* Helpers provided by the build environment.  */
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) returns seq_t */

/*  Static cache of computed sequence values.  */
static seq_t *g_cache       = NULL;   /* pointer to cached values          */
static size_t g_cache_size  = 0;      /* number of values actually stored  */
static size_t g_cache_cap   = 0;      /* allocated capacity in elements    */

/*  Static storage for the most recently generated row (previous row).  */
static seq_t *g_prev_row    = NULL;   /* array of length g_prev_len        */
static size_t g_prev_len    = 0;      /* length of g_prev_row (row index+1) */

static void ensure_cache_capacity(size_t required)
{
    if (required <= g_cache_cap)
        return;
    size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 64;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    g_cache = tmp;
    g_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /*  Lazy initialisation of the first row (row 0).  */
    if (g_cache_size == 0) {
        /* initialise the cache */
        g_cache_cap = 64;
        g_cache = (seq_t *)malloc(g_cache_cap * sizeof(seq_t));
        g_cache_size = 0;

        /* initialise the first row: [1]  */
        g_prev_len = 1;
        g_prev_row = (seq_t *)malloc(g_prev_len * sizeof(seq_t));
        g_prev_row[0] = (seq_t)1;

        /* store first value in cache */
        ensure_cache_capacity(1);
        g_cache[g_cache_size++] = g_prev_row[0];
    }

    /*  Generate rows until the cache contains at least n+1 terms.  */
    while (g_cache_size <= (size_t)n) {
        /* New row index is g_prev_len (previous row index+1).  */
        size_t new_len = g_prev_len + 1;          /* length of newly generated row */
        size_t prev_len = g_prev_len;             /* length of previous row        */
        /* Allocate array for the new row.  */
        seq_t *curr_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        /*  Pre‑compute powers of 4 up to prev_len-1.  */
        seq_t *pow4 = (seq_t *)malloc(prev_len * sizeof(seq_t));
        pow4[0] = (seq_t)1;
        for (size_t d = 1; d < prev_len; ++d) {
            pow4[d] = pow4[d - 1] * ((seq_t)4);
        }

        /*  Compute intermediate array B[t] = Σ_{j=t}^{prev_len-1} A[j] * C(j,t) * 4^{j-t}.  */
        seq_t *B = (seq_t *)malloc(prev_len * sizeof(seq_t));
        for (size_t t = 0; t < prev_len; ++t) {
            seq_t sum = (seq_t)0;
            for (size_t j = t; j < prev_len; ++j) {
                seq_t coeff = g_prev_row[j];
                seq_t choose = binomial(j, t);
                seq_t mult = coeff * choose * pow4[j - t];
                sum += mult;
            }
            B[t] = sum;
        }

        /*  Construct the new row: curr_row[k] = 3*B[k] + B[k-1] (with B[-1]=0).  */
        for (size_t k = 0; k < new_len; ++k) {
            seq_t part1 = (k < prev_len) ? B[k] * ((seq_t)3) : (seq_t)0;
            seq_t part2 = (k >= 1 && (k - 1) < prev_len) ? B[k - 1] : (seq_t)0;
            curr_row[k] = part1 + part2;
        }

        /*  Clean up temporary arrays.  */
        free(g_prev_row);
        free(pow4);
        free(B);

        /*  Append the new row to the cache.  */
        for (size_t i = 0; i < new_len; ++i) {
            ensure_cache_capacity(g_cache_size + 1);
            g_cache[g_cache_size++] = curr_row[i];
        }

        /*  Prepare for next iteration.  */
        g_prev_row = curr_row;
        g_prev_len = new_len;
    }

    /*  Return the required term.  */
    return g_cache[(size_t)n];
}
