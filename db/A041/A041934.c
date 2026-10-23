/*
 * A041934  Numerators of continued fraction convergents to sqrt(490).
 * https://oeis.org/A041934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041934: Numerators of continued fraction convergents to sqrt(490)
   Recurrence: p_0 = a_0 = 22
                p_1 = a_1 * p_0 + 1
                p_k = a_k * p_{k-1} + p_{k-2}   for k >= 2
   where the continued‑fraction coefficients a_k are
   a_0 = 22, a_1 = 7, a_2 = 2, a_3 = 1, a_4 = 4, a_5 = 4, a_6 = 4,
   a_7 = 1, a_8 = 2, a_9 = 7, a_10 = 44, and then the block of length
   10 repeats cyclically. */

#include <stdlib.h>
#include <stdint.h>

/* Return the k-th continued‑fraction coefficient for sqrt(490) (k >= 0) */
static seq_t cf_coeff(idx_t k)
{
    /* a0 followed by the cyclic period of length 10 */
    static const seq_t period[11] = {
        22, /* a0 */
        7,  /* a1 */
        2,  /* a2 */
        1,  /* a3 */
        4,  /* a4 */
        4,  /* a5 */
        4,  /* a6 */
        1,  /* a7 */
        2,  /* a8 */
        7,  /* a9 */
        44  /* a10 */
    };

    if (k == 0)
        return period[0];

    /* indices 1 .. 10 repeat */
    size_t idx = ((size_t)(k) - 1) % 10 + 1;
    return period[idx];
}

/* Cached numerators of convergents.  The array grows as needed. */
static seq_t *cached_terms = NULL;
static size_t cache_cap   = 0;   /* allocated capacity in entries */
static size_t cache_len   = 0;   /* number of terms already computed */

/* Compute the n-th numerator (n >= 0). */
seq_t a(idx_t n)
{
    /* Use size_t for array indices.  The cast is safe because
       the build uses 64‑bit size_t. */
    size_t nsize = (size_t)n;

    /* Ensure capacity.  Grow to a power of two >= nsize+1. */
    if (nsize + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < nsize + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – terminate the program as the sequence
               cannot be computed. */
            abort();
        }
        cached_terms = tmp;
        cache_cap    = new_cap;
    }

    /* Compute missing terms up to nsize. */
    for (size_t k = cache_len; k <= nsize; ++k) {
        seq_t ak = cf_coeff((idx_t)k);
        if (k == 0) {
            cached_terms[0] = ak;                /* p_0 = a_0 */
        } else if (k == 1) {
            cached_terms[1] = ak * cached_terms[0] + (seq_t)1; /* p_1 */
        } else {
            cached_terms[k] = ak * cached_terms[k - 1] + cached_terms[k - 2];
        }
    }

    cache_len = nsize + 1;
    return cached_terms[nsize];
}
