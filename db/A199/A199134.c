/*
 * A199134  Indices of Greedy Queens (see A065188) below main diagonal.
 * https://oeis.org/A199134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 9096, u32 n <= 9096, u64 n <= 9096,
 *            u128 n <= 9096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy Queen algorithm: returns the index of the nth queen that lies
   below the main diagonal (A199134).  The function memoises the board
   state across calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>        /* size_t */

/* Limits sufficient for all required n (e.g. n <= 10000).  They can be
   increased if larger values are needed. */
#define MAX_COLS   200000          /* maximum column value used */
#define DIAG_OFF   MAX_COLS         /* offset for negative diagonal indices */
#define MAX_DIAG  (2 * MAX_COLS + 1)  /* size of diagonal occupation tables */

/* static state kept between calls */
static idx_t    computed_rows  = 0;                     /* rows already processed   */
static uint8_t  used_col[MAX_COLS + 1] = {0};           /* column usage flag        */
static uint8_t  diag1[MAX_DIAG]       = {0};            /* r - c + DIAG_OFF occupied */
static uint8_t  diag2[MAX_DIAG]       = {0};            /* r + c occupied           */

static idx_t   *below_indices = NULL;                  /* rows with queen < row    */
static size_t  below_cap   = 0;                        /* capacity of below_indices*/
static size_t  below_len   = 0;                        /* current number of entries*/

/* Allocate (or grow) the list that stores rows where the queen is below the main diagonal */
static void
ensure_below_capacity(size_t needed)
{
    if (needed <= below_cap) return;
    size_t new_cap = below_cap ? below_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(below_indices, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);                      /* memory allocation failed */
    below_indices = tmp;
    below_cap = new_cap;
}

/* Main entry function defined with external linkage */
seq_t
a(idx_t n)
{
    /* Validate input (n >= 1).  The function behavior below the offset is
       unspecified, so we simply return 0 for n == 0. */
    if (n == 0) return 0;

    /* Generate rows until we have n entries in below_indices */
    while (below_len < (size_t)n)
    {
        /* ---- generate next greedy queen ---- */
        ++computed_rows;
        idx_t r = computed_rows;
        idx_t c = 1;

        /* Find the smallest column that does not conflict */
        while (1)
        {
            if (c > MAX_COLS) exit(EXIT_FAILURE);          /* safety guard */
            if (used_col[c]) { ++c; continue; }

            /* Compute diagonal indices */
            size_t d1 = (size_t)(r - c + DIAG_OFF);
            size_t d2 = (size_t)(r + c);
            if (d1 >= MAX_DIAG || d2 >= MAX_DIAG) exit(EXIT_FAILURE); /* safety guard */

            if (!diag1[d1] && !diag2[d2]) break;          /* found a free square */
            ++c;
        }

        /* Mark the queen's position as occupied */
        used_col[c] = 1;
        diag1[(size_t)(r - c + DIAG_OFF)] = 1;
        diag2[(size_t)(r + c)] = 1;

        /* Store the index if the queen is below the main diagonal */
        if (c < r)
        {
            ensure_below_capacity(below_len + 1);
            below_indices[below_len++] = r;
        }
    }

    /* Return the desired term (1‑based) */
    return (seq_t)below_indices[(size_t)n - 1];
}
