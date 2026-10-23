/*
 * A042378  Numerators of continued fraction convergents to sqrt(716).
 * https://oeis.org/A042378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 70231439376398*a(n-28) - a(n-56)   (n > 55).
   Initial values a(0) … a(55) are the numerators of the
   convergents of the simple continued fraction of sqrt(716). */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;          /* stored terms           */
static idx_t cache_size = 0;         /* number of terms stored */

/* Compute the first 56 convergent numerators of sqrt(716). */
static void init_cache(void)
{
    const idx_t D   = 716;
    const idx_t a0  = 26;                /* floor(sqrt(716))        */
    const size_t INIT = 56;              /* terms 0 … 55 inclusive */

    /* Allocate storage for the 56 values.  Cast to seq_t* for C++. */
    seq_t *p = (seq_t *)malloc(sizeof(seq_t) * INIT);
    if (!p) abort();

    /* Compute the continued‑fraction coefficients a_k for k = 0 … 55. */
    idx_t coeffs[INIT];
    coeffs[0] = a0;
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a   = a0;
    for (idx_t i = 1; i < INIT; ++i) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        coeffs[i] = a;
    }

    /* Compute convergent numerators: p_k = a_k * p_{k-1} + p_{k-2}.  */
    seq_t pprev2 = 0;   /* p_{-2} */
    seq_t pprev1 = 1;   /* p_{-1} */
    for (idx_t k = 0; k < INIT; ++k) {
        seq_t coeff = (seq_t)coeffs[k];
        seq_t pk     = coeff * pprev1 + pprev2;
        p[k]         = pk;
        pprev2       = pprev1;
        pprev1       = pk;
    }

    cache = p;
    cache_size = INIT;
}

/* Return the nth term of A042378 (0‑based indexing). */
seq_t a(idx_t n)
{
    if (!cache) init_cache();        /* lazy initialization */

    if (n < cache_size)              /* already computed */
        return cache[n];

    /* Grow the cache to hold up to index n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    const seq_t coeff = (seq_t)70231439376398ULL;   /* recurrence constant */

    for (idx_t i = cache_size; i <= n; ++i) {
        /* Recurrence uses indices i-28 and i-56, which are valid for i > 55. */
        cache[i] = coeff * cache[i - 28] - cache[i - 56];
    }
    cache_size = new_size;

    return cache[n];
}
