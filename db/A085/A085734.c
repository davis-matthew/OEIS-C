/*
 * A085734  Triangle read by rows: T(0,0) = 1, T(n,k) =
 *          Sum_{j=max(0,1-k)..n-k} (2^j)*(binomial(k+j,1+j) +
 *          binomial(k+j+1,1+j))*T(n-1,k-1+j).
 * https://oeis.org/A085734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 28, u64 n <= 77, u128 n <= 189,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A085734
 *
 *  Triangle read by rows: T(0,0)=1.
 *  Recurrence:
 *     T(n,k)=sum_{j=max(0,1-k)}^{n-k} 2^j * (C(k+j,1+j)+C(k+j+1,1+j)) * T(n-1,k-1+j)
 *  For n>0, 0 ≤ k ≤ n.
 *
 *  a(n) is the nth entry when the triangle is read row by row starting
 *  with T(0,0).  For example:
 *     a(0)=1, a(1)=2, a(2)=3, a(3)=16, a(4)=30, a(5)=15, …
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t **tri = NULL;                /* pointer to rows of the triangle   */
static idx_t  max_row = (idx_t)-1;        /* largest row that has been computed */

/* Ensure that rows 0 … r are computed and stored in tri. */
static void
ensure_rows_up_to(idx_t r)
{
    /* Nothing to do if we already have everything up to r. */
    if (max_row != (idx_t)-1 && r <= max_row) return;

    /* Resize the array of row pointers to hold up to r. */
    if (tri == NULL) {
        tri = (seq_t**)malloc(sizeof(seq_t*) * (r + 1));
    } else {
        tri = (seq_t**)realloc(tri, sizeof(seq_t*) * (r + 1));
    }

    /* Compute any missing rows. */
    idx_t start = (max_row == (idx_t)-1) ? 0 : (max_row + 1);
    for (idx_t row = start; row <= r; ++row) {
        /* Allocate storage for this row.  For C++ big‑int types we need
           constructors, for plain C we can use calloc.  */
#ifdef __cplusplus
        tri[row] = new seq_t[row + 1];
#else
        tri[row] = (seq_t*)calloc(row + 1, sizeof(seq_t));
#endif
        if (row == 0) {
            tri[0][0] = (seq_t)1;
            continue;
        }

        seq_t *prev = tri[row - 1];
        for (idx_t k = 0; k <= row; ++k) {
            seq_t sum = 0;
            idx_t j_start = (k < 1 ? 1 - k : 0);      /* max(0,1‑k) */
            idx_t j_end   = row - k;                  /* n‑k      */
            for (idx_t j = j_start; j <= j_end; ++j) {
                seq_t term  = pow2(j);                               /* 2^j           */
                term *= (binomial(k + j, j + 1) +                 /* C(k+j,1+j)    */
                         binomial(k + j + 1, j + 1));             /* C(k+j+1,1+j)  */
                term *= prev[(k - 1) + j];                         /* T(n-1,k-1+j)  */
                sum += term;
            }
            tri[row][k] = sum;
        }
    }
    max_row = r;
}

/* Binary search that returns the largest r with r*(r+1)/2 ≤ n. */
static idx_t
row_from_index(idx_t n)
{
    if (n == 0) return 0;
    idx_t low  = 0;
    idx_t high = 1;
    /* Grow high until the triangular number exceeds n.  Use __int128
       to avoid overflow when n is large. */
    while (1) {
        __int128 s = (__int128)high * (high + 1) / 2;
        if (s > n) break;
        if (high == (idx_t)-1) break;          /* guard against overflow */
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 s = (__int128)mid * (mid + 1) / 2;
        if (s <= n) low = mid;
        else        high = mid - 1;
    }
    return low;
}

/* Return the nth term of A085734, where the triangle is read row
   by row starting with T(0,0). */
seq_t
a(idx_t n)
{
    idx_t row = row_from_index(n);
    idx_t base = row * (row + 1) / 2;    /* first index of this row */
    idx_t col  = n - base;               /* column within the row      */

    ensure_rows_up_to(row);
    return tri[row][col];
}
