/*
 * A232266  Triangle where T(n,k) = number of compositions of n^2 - k^2 into
 *          sums of squares for k=0..n, n>=0, as read by rows.
 * https://oeis.org/A232266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 44, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A232266
 *
 *  T(n,k) = number of compositions of n^2-k^2 into squares.
 *
 *  We flatten the triangle row-wise:  a(n) = T(r,c) where
 *    r = floor((sqrt(8n+1)-1)/2)
 *    c = n - r(r+1)/2
 *
 *  The composition count f(m) satisfies
 *    f(0) = 1
 *    f(m) = Σ f(m - s^2)   over all squares s^2 ≤ m
 *
 *  This routine lazily pre‑computes f(m) on demand up to the maximum
 *  value required by the current call and caches the results.
 */

#include <stdlib.h>
#include <math.h>

static seq_t *f_arr = NULL;   /* cache of composition counts f(m) */
static idx_t f_max = 0;       /* highest m computed so far */

/* ensure f_arr[0..m] are filled */
static void compute_f_up_to(idx_t m)
{
    if (m <= f_max) return;
    size_t new_cap = (size_t)(m + 1);
    f_arr = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
    for (idx_t i = f_max + 1; i <= m; ++i) {
        seq_t sum = 0;
        for (idx_t s = 1; s * s <= i; ++s) {
            sum += f_arr[i - s * s];
        }
        f_arr[i] = sum;
    }
    f_max = m;
}

/* compute row index r for given flattened index n */
static inline idx_t row_from_index(idx_t n)
{
    /* r = floor((sqrt(8*n+1)-1)/2) */
    return (idx_t)((sqrt((double)8.0 * n + 1.0) - 1.0) / 2.0);
}

/* main entry point required by the judge */
seq_t a(idx_t n)
{
    /* lazily initialise cache with f(0)=1 */
    if (f_arr == NULL) {
        f_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!f_arr) return 0;            /* allocation failed */
        f_arr[0] = 1;
        f_max  = 0;
    }

    idx_t r   = row_from_index(n);
    idx_t base = r * (r + 1) / 2;          /* first index of row r */
    idx_t k   = n - base;                  /* column within the row */
    idx_t m   = r * r - k * k;             /* argument for f */

    compute_f_up_to(m);
    return f_arr[m];
}
