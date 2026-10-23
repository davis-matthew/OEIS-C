/*
 * A157212  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) - m*f(n,k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1, f(n, k) = k if k <= floor(n/2) otherwise n-k, and m =
 *          3, read by rows.
 * https://oeis.org/A157212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 58, u64 n <= 142,
 *            u128 n <= 388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A157212 – recurrence with m = 3
   T(n, k) = (3*(n-k)+1)*T(n-1,k-1) + (3*k+1)*T(n-1,k) - 3*f(n,k)*T(n-2,k-1)
   with T(n,0)=T(n,n)=1, f(n,k)=k if k≤⌊n/2⌋ else n-k
   Sequence read by rows (a(0)=1, a(1)=1, a(2)=1, ...). */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* The following helper(s) are only used for the integer square root   */
/* ------------------------------------------------------------------ */

/* Integer square root of a 64‑bit unsigned integer, truncating.     */
static inline unsigned long long isqrt_ul(unsigned long long v)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62; /* start with the highest power of two */
    while (bit > 0) {
        unsigned long long next = r | bit;
        if (next * next <= v)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/*   Globals to store the triangle rows                               */
/* ------------------------------------------------------------------ */

static seq_t **triangle_rows = NULL;  /* array of pointers to rows   */
static size_t    rows_cap     = 0;    /* capacity of triangle_rows   */
static size_t    rows_len     = 0;    /* number of rows currently stored */

/* ------------------------------------------------------------------ */
/* Grow the array of row pointers to at least a minimum capacity.    */
/* ------------------------------------------------------------------ */
static void grow_rows_cap(size_t min_cap)
{
    if (min_cap <= rows_cap)
        return;
    if (rows_cap == 0) rows_cap = 4;
    while (rows_cap < min_cap) rows_cap <<= 1;
    triangle_rows = (seq_t **)realloc(triangle_rows, rows_cap * sizeof(seq_t *));
}

/* ------------------------------------------------------------------ */
/* Compute rows of the triangle up to (and including) the row number  */
/*  r_needed.  The rows are stored in triangle_rows[r] with          */
/*  length (r+1).  Rows 0 and 1 are the base cases, all other rows   */
/*  are built from the recurrence.  All arithmetic is performed in   */
/*  seq_t.                                                            */
/* ------------------------------------------------------------------ */
static void ensure_rows_to(size_t r_needed)
{
    while (rows_len <= r_needed) {
        size_t r = rows_len;            /* the row we are going to compute */

        grow_rows_cap(rows_len + 1);
        triangle_rows[rows_len] = (seq_t *)malloc((r + 1) * sizeof(seq_t));

        if (r == 0) {                   /* row 0: [1] */
            triangle_rows[r][0] = 1;
        } else if (r == 1) {            /* row 1: [1,1] */
            triangle_rows[r][0] = 1;
            triangle_rows[r][1] = 1;
        } else {
            /* Compute the interior entries using the recurrence. */
            seq_t *prev = triangle_rows[r - 1];
            seq_t *prevprev = triangle_rows[r - 2];
            for (size_t k = 0; k <= r; ++k) {
                if (k == 0 || k == r) {
                    triangle_rows[r][k] = 1;
                } else {
                    /* m = 3 */
                    seq_t part1 = ((seq_t)3 * (r - k) + 1) * prev[k - 1];
                    seq_t part2 = ((seq_t)3 * k + 1) * prev[k];
                    size_t f = (k <= r / 2) ? k : r - k;
                    seq_t part3 = ((seq_t)3 * f) * prevprev[k - 1];
                    triangle_rows[r][k] = part1 + part2 - part3;
                }
            }
        }

        rows_len++;                     /* one more row stored */
    }
}

/* ------------------------------------------------------------------ */
/* Converts a global index n (flattened by rows) into the corresponding*/
/* row r and column k within that row.  Returns r and sets *k.        */
/* ------------------------------------------------------------------ */
static size_t index_to_rowk(idx_t n, size_t *k_out)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    size_t r = 0;
    unsigned __int128 tri = 0;
    while (1) {
        unsigned __int128 next_tri = (unsigned __int128)(r + 1) * (r + 2) / 2ULL;
        if (next_tri > (unsigned __int128)n)
            break;
        ++r;
        tri = next_tri;
    }
    size_t k = (size_t)((unsigned __int128)n - tri);
    *k_out = k;
    return r;
}

/* ------------------------------------------------------------------ */
/* Main entry point – returns the nth term of sequence A157212.        */
/* The function supports arbitrary call order and caches all computed*/
/* rows.  All operations are performed in seq_t, so the result is      */
/* exact whenever it fits in seq_t.  It complies with the specified   */
/* requirements.                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    size_t k;
    size_t r = index_to_rowk(n, &k);
    ensure_rows_to(r);
    return triangle_rows[r][k];
}
