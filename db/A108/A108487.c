/*
 * A108487  a(n) = Sum_{k=0..floor(n/2)} binomial(2n-2k,2k)10^(n-k).
 * https://oeis.org/A108487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} C(2n-2k, 2k) * 10^(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard utility for binomial coefficients */
#include "../utils/binomial.h"
#include <stdlib.h>

/* Computes A108487: a(n) = Σ_{k=0..⌊n/2⌋} C(2n-2k, 2k) * 10^(n-k)        */
seq_t a(idx_t n)
{
    /* Static cache of previously computed a(n) values */
    static seq_t *a_cache       = NULL;
    static idx_t  a_cap         = 0;   /* allocated size of a_cache   */
    static idx_t  a_size        = 0;   /* number of initialized items */

    /* Static cache of powers of 10: pow_cache[i] = 10^i            */
    static seq_t *pow_cache      = NULL;
    static idx_t  pow_cap        = 0;   /* allocated size of pow_cache  */
    static idx_t  pow_size       = 0;   /* number of initialized items  */

    /* Ensure a_cache has room for n */
    if (n >= a_cap) {
        idx_t new_cap = a_cap ? a_cap : 16;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Ensure pow_cache has room for n */
    if (n >= pow_cap) {
        idx_t new_cap = pow_cap ? pow_cap : 16;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow_cache = tmp;
        pow_cap   = new_cap;
    }

    /* Fill pow_cache up to exponent n */
    if (pow_size == 0) {               /* initialize pow_cache[0] = 1 */
        pow_cache[0] = (seq_t)1;
        pow_size = 1;
    }
    for (idx_t i = pow_size; i <= n; ++i)
        pow_cache[i] = pow_cache[i - 1] * (seq_t)10;
    pow_size = n + 1;

    /* Compute a(n) values up to n */
    for (idx_t i = a_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)      val = (seq_t)10;
        else if (i == 2)      val = (seq_t)110;
        else if (i == 3)      val = (seq_t)1600;
        else {
            idx_t maxk = i >> 1;          /* floor(i/2)   */
            val = (seq_t)0;
            for (idx_t k = 0; k <= maxk; ++k) {
                /* C(2*i - 2*k, 2*k)  */
                seq_t bin = binomial((idx_t)(2 * i - 2 * k),
                                     (idx_t)(2 * k));
                /* 10^(i - k) from pow_cache              */
                seq_t ten = pow_cache[i - k];
                val = val + bin * ten;
            }
        }
        a_cache[i] = val;
    }
    a_size = n + 1;

    return a_cache[n];
}
