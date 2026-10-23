/*
 * A041856  Numerators of continued fraction convergents to sqrt(450).
 * https://oeis.org/A041856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to √450.
   a(0) = 21 , a(-1) = 1
   a(n) = c_n * a(n-1) + a(n-2)   for n ≥ 1
   where the partial quotients c_n are periodic:
   c_0 = 21, c_1…c_8 = 4,1,2,4,2,1,4,42 and c_{n+8}=c_n for n≥1. */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;   /* array of computed a(n) */
static size_t cache_len = 0;  /* number of elements currently stored */

/* Periodic coefficients of the continued fraction after the first term */
static const int period[8] = {4, 1, 2, 4, 2, 1, 4, 42};

/* Ensure that entries up to index n (inclusive) are computed and cached. */
static void a_cache_up_to(idx_t n)
{
    size_t required = (size_t)n + 1;           /* needed size in elements */
    if (required <= cache_len)
        return;                                /* already computed */

    /* Allocate or grow the cache */
    seq_t *tmp;
    if (cache == NULL) {
        tmp = (seq_t *)malloc(required * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
    }
    if (tmp == NULL)
        return;                                /* allocation failure – give up */

    cache = tmp;

    /* Initialize the first element if this is the first allocation */
    if (cache_len == 0) {
        cache[0] = 21;                         /* a(0) = 21 */
        cache_len = 1;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        int coeff_int;
        if (i == 0) {
            /* c_0 is 21, but a(0) is already set */
            coeff_int = 21;
        } else {
            coeff_int = period[(size_t)((i - 1) % 8)];
        }
        seq_t coeff = (seq_t)coeff_int;
        seq_t prev1 = cache[i - 1];
        seq_t prev2;
        if (i == 1) {
            prev2 = (seq_t)1;                     /* a(-1) = 1 */
        } else {
            prev2 = cache[i - 2];
        }
        cache[i] = coeff * prev1 + prev2;
    }

    cache_len = required;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    a_cache_up_to(n);
    if (n < cache_len)
        return cache[n];
    return 0;                                        /* allocation failure */
}
