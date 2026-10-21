/*
 * A067990  Triangle A067979 with rows read backwards.
 * https://oeis.org/A067990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by listing the entries of triangle A067979
 *            row‑by‑row (index 0 gives the first entry of row 0, index 1 the
 *            first entry of row 1, index 2 the second entry of row 1, etc.).
 *            The triangle entry a(r,m) is given by a(r,m) =
 *            (r‑m+1)·L(m+1)·F(r‑m) + ((r‑m+1)·L(m+1) +
 *            (r‑m)·L(m))·F(r‑m+1)...
 * Range:     u8 n <= 20, u16 n <= 135, u32 n <= 702, u64 n <= 3402,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* -------------  Fibonacci cache -------------- */

static idx_t fib_size  = 0;               /* number of initialized entries   */
static idx_t fib_cap   = 0;               /* capacity of array               */
static seq_t *fib      = NULL;            /* dynamic array of Fib numbers    */

static void ensure_fib(idx_t n)
{
    if (fib_size > n) return;               /* already have up to n           */

    /* grow capacity */
    if (fib_cap == 0) fib_cap = 16;
    while (fib_cap <= n) fib_cap <<= 1;     /* double until big enough       */

    /* allocate or reallocate */
#ifdef __cplusplus
    seq_t *newp = new seq_t[fib_cap];
#else
    seq_t *newp = (seq_t*)malloc(fib_cap * sizeof(seq_t));
#endif
    if (!newp) return;                      /* allocation failed – abort      */
    if (fib) {
        for (idx_t i = 0; i < fib_size; ++i) newp[i] = fib[i];
#ifdef __cplusplus
        delete[] fib;
#else
        free(fib);
#endif
    }
    fib = newp;
    fib_cap = fib_cap;

    /* initialise first two Fibonacci numbers if needed */
    if (fib_size == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_size = 2;
    }

    /* compute missing entries */
    for (idx_t i = fib_size; i <= n; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    if (n + 1 > fib_size) fib_size = n + 1;
}

/* compute Lucas number L(k) using Fibonacci array
   L(0) = 2,  for k>=1, L(k) = F(k-1)+F(k+1)          */
static seq_t get_L(idx_t k)
{
    if (k == 0) return (seq_t)2;
    ensure_fib(k + 1);                       /* ensure F(k-1) and F(k+1)      */
    return fib[k-1] + fib[k+1];
}

/* ----------  triangle entry  ----------------- */
static seq_t a_row_col(idx_t r, idx_t m)
{
    idx_t nm = r - m;                        /* n - m  */
    ensure_fib(r + 2);                       /* need F up to r+2 for L(m+1)   */

    seq_t nm1 = (seq_t)(nm + 1);             /* (n-m+1) */

    seq_t L_m1 = get_L(m + 1);               /* L(m+1)  */
    seq_t L_m  = get_L(m);                   /* L(m)    */

    seq_t term1 = nm1 * L_m1 * fib[nm];
    seq_t term2 = ((nm1 * L_m1) + (nm * L_m)) * fib[nm + 1];
    return term1 + term2;
}

/* -----  linear index -> (row, col)  --------- */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* find largest r with r*(r+1)/2 <= n  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;  /* safe for n up to 10^5      */
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* -------------  public interface ------------- */
seq_t a(idx_t n)
{
    idx_t r, m;
    index_to_row_col(n, &r, &m);
    return a_row_col(r, m);
}
