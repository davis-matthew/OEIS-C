/*
 * A079278  Define a rational sequence {b(n)} as b(1) = 1, b(n) = b(n-1) +
 *          1/(1 + 1/b(n-1)) for n > 1; a(n) is the denominator of b(n).
 * https://oeis.org/A079278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1) = 1; for n > 1 * b(1) = 1 * b(n) = b(n-1) + 1/(1 +
 *            1/b(n-1)) * a(n) is the denominator of the fraction b(n) in
 *            lowest terms. * The recurrence for the fraction is: * if b(n-1)
 *            = p/q (p and q coprime) * then b(n) = p*(p+2q) / (q*(p+q)) * and
 *            the fraction is already in lowest terms
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Maintain the current fraction (num/den) and a cache of denominators */
    static seq_t num = 1;          /* current numerator */
    static seq_t den = 1;          /* current denominator */
    static seq_t *den_cache = NULL;/* array holding denominators a(1)…a(cache_len) */
    static size_t cache_len = 0;   /* number of denominators cached */
    static size_t cache_cap = 0;   /* capacity of den_cache */

    /* Initialise cache the first time */
    if (cache_len == 0) {
        cache_cap = 4;
        den_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!den_cache) return 0;            /* allocation failed */
        den_cache[0] = 1;                    /* a(1) = 1 */
        cache_len = 1;
    }

    /* If requested n is already cached, return it immediately */
    if (n <= cache_len)
        return den_cache[n - 1];

    /* Extend the cache up to n */
    while (cache_len < n) {
        /* Compute the next fraction b(n) = p*(p+2q) / (q*(p+q)) */
        seq_t p = num;
        seq_t q = den;

        seq_t p_plus_q  = p + q;          /* p + q      */
        seq_t p_plus_2q = p + q + q;      /* p + 2*q    */

        seq_t new_num = p * p_plus_2q;    /* p * (p+2q) */
        seq_t new_den = q * p_plus_q;     /* q * (p+q)  */

        /* Update current fraction */
        num = new_num;
        den = new_den;

        /* Ensure the cache has enough space */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            den_cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the new denominator */
        den_cache[cache_len] = den;
        cache_len++;
    }

    return den_cache[n - 1];
}
