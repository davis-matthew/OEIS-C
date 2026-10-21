/*
 * A220354  Irregular triangle T(n,k), read by rows; row n gives coefficients
 *          in expansion of P_n(x), which is defined by: P_0(x) = 1, P_n(x) =
 *          P_(n-1)(x)+x if n odd, P_n(x) = P_(n-1)(x)*x if n even.
 * https://oeis.org/A220354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth coefficient in the irregular triangle T(n,k)
 *            defined by the polynomials P0(x) = 1 Pn(x) = Pn-1(x) + x if n is
 *            odd Pn(x) = Pn-1(x) * x if n is even The triangle is read
 *            row‑by‑row, row 0 first, then row 1, etc. Thus a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=0, a(4)=1, a(5)=1, … This function bu...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Cached flattened sequence and its state */
static seq_t *flat_cache   = NULL;  /* array of all coefficients so far */
static size_t  cache_len   = 0;      /* number of coefficients stored */
static size_t  cache_cap   = 0;      /* allocated capacity */

/* Current polynomial state */
static seq_t *poly        = NULL;   /* array of coefficients of P_{cur_row} */
static size_t  poly_len   = 0;      /* length of that array */
static idx_t   next_row   = 0;      /* row number to compute next */

/* Allocate or grow an array of seq_t */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    /* double capacity until enough */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(flat_cache, sizeof(seq_t) * new_cap);
    flat_cache = tmp;
    cache_cap  = new_cap;
}

/* Append a row of coefficients to the cache */
static void append_row(const seq_t *row, size_t len)
{
    grow_cache(cache_len + len);
    for (size_t i = 0; i < len; ++i)
        flat_cache[cache_len++] = row[i];
}

/* Main entry function */
seq_t a(idx_t n)
{
    /* lazy initialization on first call */
    if (flat_cache == NULL) {
        cache_cap = 16;
        flat_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);

        /* Initialize P0(x) = 1 */
        poly_len = 1;
        poly = (seq_t *)malloc(sizeof(seq_t) * poly_len);
        poly[0] = (seq_t)1;

        /* Store first element */
        flat_cache[cache_len++] = poly[0];
        next_row = 1;          /* next row to compute is row 1 */
    }

    /* Generate rows until we have enough terms */
    while (cache_len <= (size_t)n) {
        seq_t *new_poly;
        size_t new_len;

        if (next_row & 1) {  /* odd row: P = old + x */
            new_len = (poly_len >= 2) ? poly_len : 2;
            new_poly = (seq_t *)malloc(sizeof(seq_t) * new_len);
            for (size_t i = 0; i < new_len; ++i)
                new_poly[i] = (i < poly_len) ? poly[i] : (seq_t)0;
            new_poly[1] += (seq_t)1;      /* increase coefficient of x by 1 */
        } else {            /* even row: P = old * x */
            new_len = poly_len + 1;
            new_poly = (seq_t *)malloc(sizeof(seq_t) * new_len);
            new_poly[0] = (seq_t)0;
            for (size_t i = 1; i < new_len; ++i)
                new_poly[i] = poly[i - 1];
        }

        /* Free old polynomial and replace */
        free(poly);
        poly     = new_poly;
        poly_len = new_len;

        /* Store the new row into the cache */
        append_row(poly, poly_len);

        ++next_row;
    }

    return flat_cache[(size_t)n];
}
