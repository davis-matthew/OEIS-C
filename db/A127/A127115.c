/*
 * A127115  n! in base 8.
 * https://oeis.org/A127115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal integer obtained from the base‑8
 *            representation of n!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/*--- dynamic digit buffer helpers -------------------------------------*/
static void grow_digits(uint8_t **buf, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t newcap = (*cap) ? (*cap) : 64;
    while (newcap < needed)
        newcap <<= 1;
    *buf = (uint8_t *)realloc(*buf, newcap * sizeof(uint8_t));
    *cap = newcap;
}

/*--- factorial cache ---------------------------------------------------*/
static seq_t *fac_cache = NULL;      /* cache of factorial values */
static idx_t   cache_computed = 0;   /* highest n that has been cached */
static size_t  cache_cap     = 0;     /* allocated size of fac_cache */

/* Ensure that fac_cache[n] is computed and available for all 0..n */
static void ensure_factorial(idx_t n)
{
    /* Lazy initialisation of cache[0] = 1 */
    if (cache_computed == 0 && fac_cache == NULL) {
        if (cache_cap == 0) {
            cache_cap = 1;
            fac_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        fac_cache[0] = (seq_t)1;
    }

    if (n <= cache_computed)
        return;   /* already available */

    /* Grow the cache if necessary */
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    fac_cache = (seq_t *)realloc(fac_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* Compute missing factorials incrementally */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;
    }
    cache_computed = n;
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* Compute factorial(n) if not cached yet */
    ensure_factorial(n);
    seq_t f_val = fac_cache[n];       /* copy, because we'll modify it */

    /* Extract base‑8 digits (least‑significant first) */
    uint8_t *digits = NULL;
    size_t  cap = 0;
    size_t  len = 0;

    while (f_val != (seq_t)0) {
        uint8_t d = (uint8_t)(f_val % (seq_t)8);
        grow_digits(&digits, &cap, len + 1);
        digits[len++] = d;
        f_val /= (seq_t)8;
    }

    /* For n = 0, 1 we get len == 1 with digit 1, no special case needed */

    /* Build the decimal number from the most‑significant digit */
    seq_t result = (seq_t)0;
    for (size_t i = len; i-- > 0;) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
