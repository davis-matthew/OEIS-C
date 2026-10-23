/*
 * A123361  Triangle read by rows: T(n,k) = coefficient of x^k in the
 *          polynomial p[n,x] defined by p[0,x]=1, p[1,x]=1+x and
 *          p[n,x]=(1+x)(2-x)(3-x)...(n-x) for n >= 2 (0 <= k <= n).
 * https://oeis.org/A123361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 35, i32 n <= 90, i64 n <= 230,
 *            i128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A123361 – Triangle read by rows of the coefficients of
    p[n,x] = (1+x)(2-x)(3-x)…(n-x)  (p[0,x]=1, p[1,x]=1+x)
    The sequence a(n) is the triangular array read sequentially.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache       = NULL;   /* linearised triangle */
static size_t cache_size  = 0;      /* number of elements stored */
static size_t cache_cap   = 0;      /* current allocation capacity */

static seq_t *coeff       = NULL;   /* coefficients of the current row */
static idx_t  coeff_deg   = (idx_t)-1; /* last row index computed (-1 = uninitialised) */
static size_t coeff_cap   = 0;      /* allocation capacity of coeff */

static inline void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static inline void ensure_coeff_capacity(size_t need)
{
    if (need <= coeff_cap)
        return;
    size_t new_cap = coeff_cap ? coeff_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
    coeff = tmp;
    coeff_cap = new_cap;
}

/* extend the triangle up to row r (inclusive) */
static void extend_to_row(idx_t r)
{
    /* initialise row 0 if needed */
    if (coeff_deg == (idx_t)-1) {
        ensure_coeff_capacity(1);
        coeff[0] = (seq_t)1;
        coeff_deg = 0;

        ensure_cache_capacity(1);
        cache[0] = coeff[0];
        cache_size = 1;
    }

    while (coeff_deg < r) {
        idx_t n = coeff_deg + 1;       /* next row number */

        if (n == 1) {
            /* p[1,x] = 1 + x; coefficients [1,1] */
            ensure_coeff_capacity(2);
            coeff[1] = coeff[0];
            /* append both coefficients of the new row */
            ensure_cache_capacity(cache_size + 2);
            cache[cache_size++] = coeff[0];
            cache[cache_size++] = coeff[1];
        } else { /* n >= 2: p[n,x] = p[n-1,x] * (n - x) */
            ensure_coeff_capacity(n + 1);

            /* new[n] = - old[n-1] */
            coeff[n] = - coeff[n-1];

            /* for k = n-1 down to 1: new[k] = n*old[k] - old[k-1] */
            for (idx_t k = n-1; k > 0; --k) {
                coeff[k] = coeff[k] * (seq_t)n - coeff[k-1];
            }

            /* new[0] = n * old[0] */
            coeff[0] = coeff[0] * (seq_t)n;

            /* append the new row's coefficients to the cache */
            ensure_cache_capacity(cache_size + n + 1);
            for (idx_t k = 0; k <= n; ++k) {
                cache[cache_size++] = coeff[k];
            }
        }
        coeff_deg = n;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find the row that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    if (cache_size <= n)
        extend_to_row(r);

    return cache[n];
}
