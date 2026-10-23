/*
 * A026807  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n in which every part is >=k, for k=1,2,...,n.
 * https://oeis.org/A026807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term obtained by flattening the triangular array *
 *            T(r,k) read by rows: a(1) = T(1,1), a(2) = T(2,1), a(3) =
 *            T(2,2), ... * Recurrence for the array: * T(r,k) = 1 if 2*k > r
 *            * T(r,k+1) + T(r-k,k) otherwise * (with the convention T(r,k)=0
 *            for k>r) * The function below implements this de...
 * Range:     u8 n <= 136, u16 n <= 946, u32 n <= 8128, u64 n <= 8816,
 *            u128 n <= 8816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*     Memoised storage of the triangular array T(r,k) (1‑based)     */
/* ------------------------------------------------------------------ */
static seq_t *t_cache   = NULL;  /* linear array of values               */
static idx_t  t_rows    = 0;     /* number of rows currently cached       */

/* Ensure that rows 1..rows are present in t_cache */
static void t_cache_ensure(idx_t rows)
{
    if (rows <= t_rows) return;
    idx_t old_size = t_rows * (t_rows + 1) / 2;
    idx_t new_size = rows * (rows + 1) / 2;

    t_cache = (seq_t *)realloc(t_cache, new_size * sizeof(seq_t));
    if (!t_cache) abort(); /* fatal error if allocation fails */

    for (idx_t i = old_size; i < new_size; ++i) t_cache[i] = 0;

    t_rows = rows;
}

/* ------------------------------------------------------------------ */
/*   Recursive computation of T(r,k) with memoisation                */
/* ------------------------------------------------------------------ */
static seq_t T_val(idx_t r, idx_t k)
{
    if (k > r) return 0;                       /* unused elements */
    t_cache_ensure(r);
    idx_t pos = (r - 1) * r / 2 + (k - 1);     /* linear index (0‑based) */

    seq_t val = t_cache[pos];
    if (val != 0) return val;                  /* already computed */

    if ((seq_t)2 * k > (seq_t)r) {             /* base case     */
        val = 1;
    } else {
        val = T_val(r, k + 1) + T_val(r - k, k);
    }
    t_cache[pos] = val;
    return val;
}

/* ------------------------------------------------------------------ */
/*   Public interface:  return a(n) for n ≥ 1                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* below offset */

    /* find the row r such that r*(r+1)/2 ≥ n */
    long double disc = sqrtl((long double)1.0L + 8.0L * (long double)n);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);    /* floor */
    if (r * (r + 1) / 2 < n) ++r;              /* adjust to ceil */

    /* column k within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    return T_val(r, k);
}
