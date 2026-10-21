/*
 * A053259  Coefficients of the '5th-order' mock theta function phi_1(q).
 * https://oeis.org/A053259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in \sum_{m>=0} q^{(m+1)^2}
 *            \prod_{k=1}^{m} (1+q^{2k-1})
 * Range:     u8 n <= 287, u16 n <= 753, u32 n <= 2279, u64 n <= 7650,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *seq_cache = NULL;     /* cached values of a(n) */
static size_t seq_cache_size = 0;   /* number of elements stored in cache */

/* Compute a(0) … a(max_n) and store them in seq_cache. */
static void compute_table(idx_t max_n)
{
    if (max_n + 1 <= seq_cache_size)
        return;   /* already up to date */

    size_t len = (size_t)max_n + 1;

    /* allocate new cache, zero‑initialized */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[len];
    for (size_t i = 0; i < len; ++i)
        new_cache[i] = (seq_t)0;
#else
    new_cache = (seq_t*)calloc(len, sizeof(seq_t));
    if (!new_cache) return;   /* allocation failure – leave cache unchanged */
#endif

    /* floor of sqrt(max_n) */
    idx_t sqrt_max = 0;
    while (1) {
        idx_t inc = sqrt_max + 1;
        /* check overflow: inc*inc <= max_n */
        if (inc > max_n / inc) break;
        if (inc * inc > max_n) break;
        sqrt_max = inc;
    }

    if (sqrt_max > 0) {
        idx_t n_max = sqrt_max - 1;    /* largest m with (m+1)^2 <= max_n */

        /* polynomial for current P_m */
        seq_t *poly;
        seq_t *nextpoly;
#ifdef __cplusplus
        poly = new seq_t[len];
        nextpoly = new seq_t[len];
        for (size_t i = 0; i < len; ++i) {
            poly[i]      = (seq_t)0;
            nextpoly[i]  = (seq_t)0;
        }
#else
        poly      = (seq_t*)calloc(len, sizeof(seq_t));
        nextpoly  = (seq_t*)calloc(len, sizeof(seq_t));
        if (!poly || !nextpoly) {
            free(poly); free(nextpoly);
            free(new_cache);
            return;
        }
#endif

        /* P_0 = q^1 */
        if (len > 1)
            poly[1] = (seq_t)1;

        for (idx_t m = 0; m <= n_max; ++m) {
            /* add current polynomial to overall coefficients */
            for (idx_t i = 0; i <= max_n; ++i)
                new_cache[i] += poly[i];

            /* prepare next polynomial: P_{m+1} = shift(poly,2m+3)+shift(poly,4m+4) */
            /* zero nextpoly */
#ifdef __cplusplus
            for (size_t i = 0; i < len; ++i)
                nextpoly[i] = (seq_t)0;
#else
            memset(nextpoly, 0, len * sizeof(seq_t));
#endif
            idx_t shift1 = 2 * m + 3;
            idx_t shift2 = 4 * m + 4;
            for (idx_t i = 0; i <= max_n; ++i) {
                idx_t j = i + shift1;
                if (j <= max_n)
                    nextpoly[j] += poly[i];
                j = i + shift2;
                if (j <= max_n)
                    nextpoly[j] += poly[i];
            }

            /* swap for next iteration */
            seq_t *tmp = poly;
            poly = nextpoly;
            nextpoly = tmp;
        }

        /* free temporary polynomials */
#ifdef __cplusplus
        delete[] poly;
        delete[] nextpoly;
#else
        free(poly);
        free(nextpoly);
#endif
    }

    /* replace old cache */
    if (seq_cache)
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
    seq_cache = new_cache;
    seq_cache_size = len;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n >= seq_cache_size)
        compute_table(n);
    return seq_cache[n];
}
