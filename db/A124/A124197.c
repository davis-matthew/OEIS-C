/*
 * A124197  Number of subsets S of {1,2,3,...,n}, including the empty subset,
 *          such that if x and y are in S with x<y and x+y even, then (x+y)/2
 *          is also in S.
 * https://oeis.org/A124197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n)=2*a(n-1)-a(n-2)+A060831(n-1) for n≥2, where
 *            A060831(k)=number of odd divisors of k
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *a_cache       = NULL;   /* a[0..max_n]            */
    static idx_t *spf_cache     = NULL;   /* smallest prime factor  */
    static idx_t *odd_div_cache = NULL;   /* odd divisor count      */
    static idx_t computed_limit = 0;      /* highest n computed    */

    /* if nothing computed yet, or we need a larger array   */
    if (n > computed_limit || a_cache == NULL) {
        /* new size (ensure at least 1 for n==0)              */
        size_t new_size = (size_t)(n + 1);
        if (new_size < 1) new_size = 1; /* defensive for n==0 */

        /* allocate / grow the arrays                     */
        a_cache       = (seq_t *)   realloc(a_cache,       new_size * sizeof(seq_t));
        spf_cache     = (idx_t *)   realloc(spf_cache,     new_size * sizeof(idx_t));
        odd_div_cache = (idx_t *)   realloc(odd_div_cache, new_size * sizeof(idx_t));

        /* initialise the sieve of smallest prime factors */
        for (idx_t i = 0; i <= n; ++i) {
            spf_cache[i] = i;
        }
        for (idx_t i = 2; i * i <= n; ++i) {
            if (spf_cache[i] == i) {            /* i is prime           */
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf_cache[j] == j) {
                        spf_cache[j] = i;
                    }
                }
            }
        }

        /* compute odd divisor counts for 1 .. n            */
        for (idx_t i = 1; i <= n; ++i) {
            idx_t m = i;
            while ((m & 1) == 0) m >>= 1;        /* odd part            */
            seq_t tau = 1;
            while (m > 1) {
                idx_t p = spf_cache[m];
                idx_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                tau *= (seq_t)(e + 1);
            }
            odd_div_cache[i] = (idx_t)tau;
        }

        /* initialise a_cache first elements             */
        a_cache[0] = 1;                          /* a(0)                */
        if (n >= 1) a_cache[1] = 2;              /* a(1)                */

        /* compute a_cache for new indices (if any)        */
        for (idx_t i = 2; i <= n; ++i) {
            a_cache[i] = 2 * a_cache[i - 1] - a_cache[i - 2]
                          + (seq_t)odd_div_cache[i - 1];
        }

        computed_limit = n;
    }

    return a_cache[n];
}
