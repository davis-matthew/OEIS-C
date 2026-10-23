/*
 * A041248  Numerators of continued fraction convergents to sqrt(136).
 * https://oeis.org/A041248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(136)
   Recurrence:  a(0) = 11,  a(1) = 12
                 a(n) = c(n)*a(n‑1) + a(n‑2)   for n ≥ 2
   where coefficient c(n) = 22 if n ≡ 0 (mod 4) and n > 0, otherwise c(n) = 1. */

#include <stdlib.h>
#include <stddef.h>

/* Cache of already computed terms. */
static seq_t *cache      = NULL;  /* stored numerators a(0..len-1)      */
static idx_t   cache_len  = 0;    /* number of terms stored             */
static size_t  cache_cap  = 0;    /* allocated capacity in terms         */

/* Ensure the cache can hold terms up to index n. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Coefficient c(n) used in the recurrence.  c(0) = 11 to generate a(0). */
static inline seq_t coeff(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    if (n > 0 && (n % 4) == 0)
        return (seq_t)22;
    return (seq_t)1;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary. */
    ensure_capacity(n);

    /* Initialize recurrence with the two last computed values. */
    seq_t p_prev_prev; /* a(n-2) */
    seq_t p_prev;      /* a(n-1) */

    if (cache_len == 0) {            /* no terms stored yet */
        p_prev_prev = (seq_t)0;      /* a(-2) = 0 */
        p_prev = (seq_t)1;           /* a(-1) = 1 */
    } else if (cache_len == 1) {     /* only a(0) known */
        p_prev_prev = (seq_t)1;      /* a(-1) = 1 */
        p_prev = cache[0];           /* a(0)   */
    } else {                          /* at least two terms stored */
        p_prev_prev = cache[cache_len - 2];
        p_prev = cache[cache_len - 1];
    }

    /* Compute terms up to n inclusive. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t ck = coeff(k);
        seq_t pk = ck * p_prev + p_prev_prev;   /* recurrence a(k) */
        cache[k] = pk;

        /* Shift for next iteration. */
        p_prev_prev = p_prev;
        p_prev = pk;
    }

    cache_len = n + 1;
    return cache[n];
}
