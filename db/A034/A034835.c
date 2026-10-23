/*
 * A034835  Expansion of 1/(1-49*x)^(1/7); related to sept-factorial numbers
 *          A045754.
 * https://oeis.org/A034835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034835
 * Formula: a(0)=1, a(n)=a(n‑1)*7*(7*n-6)/n  for n≥1
 * Recurrence derived from  n*a(n)+7*(-7*n+6)*a(n‑1)=0
 * Uses gcd to cancel divisors and avoid overflow.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Static cache for memoised values. */
static seq_t *cache = NULL;         /* array of computed terms   */
static idx_t cache_size = 0;        /* number of terms stored    */
static idx_t cache_cap  = 0;        /* allocated capacity        */

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Expand cache capacity if needed. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();            /* allocation failed */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            /* Numerator: 7*(7*i-6)  (fits in seq_t) */
            seq_t num = (seq_t)7 * (((seq_t)7 * i) - (seq_t)6);
            seq_t den = (seq_t)i;        /* denominator */

            /* Cancel factors from denominator with previous term. */
            seq_t g1 = gcd(cache[i-1], den);
            seq_t a_tmp = cache[i-1] / g1;
            den /= g1;

            /* Cancel remaining factors between numerator and denominator. */
            seq_t g2 = gcd(num, den);
            num /= g2;
            den /= g2;   /* Should become 1 for integer result */

            /* Final multiplication: a_tmp * num fits in seq_t when result fits. */
            seq_t val = a_tmp * num;

            cache[i] = val;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
