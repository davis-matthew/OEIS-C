/*
 * A066032  Number of ways to write n as a product with no factor larger than
 *          m (1 <= m <=n, written row by row).
 * https://oeis.org/A066032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   a(n) = number of ways to write n as a product with no factor
 *            larger than m, read row‑by‐row from the triangle T(n,m). T(1,1)
 *            = 1; T(p,m) = 1 if p (prime) <= m else 0; For composite n:
 *            T(n,m) = ∑_{d|n, 1<d≤m} T(n/d,d) + I(n≤m) (I=1 if true else 0)
 * Range:     u8 n <= 9532, u16 n <= 9532, u32 n <= 9532, u64 n <= 9532,
 *            u128 n <= 9532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- library includes ---- */
#include <stdlib.h>
#include <stdbool.h>

/* ---- static data for memoisation ---- */
static seq_t *T = NULL;          /* flattened 2‑D array holding the triangle */
static idx_t  max_row = 0;       /* number of rows currently stored */

/* ---- helper: compute 2‑D index in the flat array ---- */
static inline idx_t flat_idx(idx_t row, idx_t col)
{
    return row * (max_row + 1) + col;   /* rows and cols are 1‑based */
}

/* ---- helper: free the currently allocated triangle ---- */
static void free_table(void)
{
    if (T == NULL) return;
#ifdef __cplusplus
    delete [] T;
#else
    free(T);
#endif
    T = NULL;
    max_row = 0;
}

/* ---- build a complete triangle up to the requested row (inclusive) ---- */
static void build_table(idx_t r)
{
    /* free old data if necessary */
    free_table();

    /* allocate new table: (r+1)x(r+1) entries, 0‑initialised */
    size_t size = (size_t)(r + 1) * (size_t)(r + 1);
#ifdef __cplusplus
    T = new seq_t[size]();          /* value‑initialise each element */
#else
    T = (seq_t*)calloc(size, sizeof(seq_t));
#endif
    if (T == NULL) { /* allocation failed; abort (use application exception handling if any) */
        return;
    }
    max_row = r;

    /* T(1,1) = 1 */
    T[flat_idx(1,1)] = (seq_t)1;

    /* compute rows in increasing order */
    for (idx_t n = 2; n <= r; ++n)
    {
        /* test primality of n */
        bool isprime = true;
        if (n > 1)
        {
            for (idx_t p = 2; p * p <= n; ++p)
            {
                if (n % p == 0)
                {
                    isprime = false;
                    break;
                }
            }
        }

        for (idx_t m = 1; m <= r; ++m)
        {
            seq_t val = 0;

            if (isprime)
            {
                if (m >= n) val = 1;          /* prime case */
            }
            else
            {
                /* composite case: sum over proper divisors d of n, with 1<d<=m */
                for (idx_t d = 2; d <= m && d <= n; ++d)
                {
                    if (n % d == 0)
                    {
                        idx_t row = n / d;
                        idx_t col = d;         /* d is the column for the sub‑problem */
                        val += T[flat_idx(row, col)];
                    }
                }
                if (m >= n) val += 1;          /* indicator I(n<=m) */
            }

            T[flat_idx(n, m)] = val;
        }
    }
}

/* ---- helper: convert global index n (1‑based) to row and column in the triangle ---- */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    /* find smallest r with r*(r+1)/2 >= n   (triangular numbers) */
    idx_t r = 1;
    idx_t tri = 1;                       /* r*(r+1)/2 for current r */
    while (tri < n)
    {
        ++r;
        tri += r;                         /* increase by next integer */
    }

    *row = r;
    *col = n - (r * (r - 1)) / 2;         /* 1‑based column */
}

/* ---- public function required by the build environment ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n=0 is unused sentinel */

    idx_t r, c;
    index_to_rowcol(n, &r, &c);

    if (r > max_row)
        build_table(r);               /* extend table if necessary */

    return T[flat_idx(r, c)];
}
