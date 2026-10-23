/*
 * A008430  Theta series of D_8 lattice.
 * https://oeis.org/A008430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 311, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008430 – Theta series of D_8 lattice
    Formula: a(n) = A000143(2n)
    (Equivalently a(n) = (theta_3(q^(1/2))^8 + theta_4(q^(1/2))^8)/2 )
*/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* caches for the coefficients of theta_3^8 and theta_4^8 */
static int64_t *r8_cache = NULL;   /* positive integer coefficients */
static int64_t *p8_cache = NULL;   /* signed integer coefficients */
static size_t  cache_len = 0;      /* number of entries stored (indices 0 … len-1) */

/* allocate an array of int64_t initialised to 0 */
static int64_t *alloc_array(size_t n)
{
    return (int64_t*)calloc(n, sizeof(int64_t));
}

/* free an array returned by alloc_array */
static void free_array(int64_t *p)
{
    free(p);
}

/* compute the arrays up to N (inclusive) */
static void compute_cache(size_t N)
{
    int64_t *a_r = alloc_array(N + 1);
    int64_t *a_p = alloc_array(N + 1);
    int64_t *b_r = alloc_array(N + 1);
    int64_t *b_p = alloc_array(N + 1);

    a_r[0] = 1;
    a_p[0] = 1;

    for (int var = 0; var < 8; ++var)
    {
        /* zero the convolution buffers */
        memset(b_r, 0, (N + 1) * sizeof(int64_t));
        memset(b_p, 0, (N + 1) * sizeof(int64_t));

        for (size_t sq = 0; sq * sq <= N; ++sq)
        {
            size_t sqval = sq * sq;
            int64_t wgt_r = (sq == 0) ? 1 : 2;
            int64_t wgt_p = (sq == 0) ? 1 : ((sq & 1) ? -2 : 2);
            size_t lim = N - sqval;

            for (size_t i = 0; i <= lim; ++i)
            {
                if (a_r[i])
                    b_r[i + sqval] += a_r[i] * wgt_r;
                if (a_p[i])
                    b_p[i + sqval] += a_p[i] * wgt_p;
            }
        }

        /* swap a and b for the next variable */
        int64_t *tmp = a_r; a_r = b_r; b_r = tmp;
        tmp = a_p; a_p = b_p; b_p = tmp;
    }

    /* free previous cache and store the new one */
    if (r8_cache) free_array(r8_cache);
    if (p8_cache) free_array(p8_cache);
    r8_cache = a_r;
    p8_cache = a_p;
    cache_len = N + 1;

    /* free the temporary buffers */
    free_array(b_r);
    free_array(b_p);
}

seq_t a(idx_t n)
{
    idx_t target = n * 2;           /* sequence is indexed by 2n */
    if (r8_cache == NULL || (size_t)target >= cache_len)
        compute_cache((size_t)target);

    int64_t value = r8_cache[(size_t)target] + p8_cache[(size_t)target];
    value /= 2;                     /* division by 2 is exact */
    return (seq_t)value;
}
