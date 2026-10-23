/*
 * A171184  G.f. satisfies: A(x) = A(x)^2 - x*A(2x).
 * https://oeis.org/A171184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A171184
 *  a(0) = 1
 *  a(n) = 2^(n-1)*a(n-1) - ∑_{k=1}^{n-1} a(k)*a(n-k)   for n > 0
 */

#include <stdlib.h>

/* external integer types are provided by the build system:
 *  typedef uint64_t idx_t;
 *  typedef uint8_t|uint16_t|uint32_t|uint64_t|unsigned __int128|... seq_t;
 */

seq_t a(idx_t n)
{
    /* static memoisation state                                   */
    static seq_t *cache    = NULL;         /* array of computed terms    */
    static size_t  cap     = 0;            /* allocated capacity         */
    static idx_t  computed  = 0;           /* first index not yet computed */

    /* establish base value for a(0) on first call                */
    if (cache == NULL) {
        cap = 16;                          /* arbitrary initial size    */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;               /* a(0)                      */
        computed = 1;                      /* a(0) has been computed    */
    }

    /* grow cache if necessary                                      */
    if ((idx_t)n + 1 > (idx_t)cap) {
        size_t new_cap = cap * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* a(0) is already available                                 */
    if (n == 0) return cache[0];

    /* compute missing values up to n (inclusive)                  */
    if (n >= computed) {
        /* maintain 2^(k-1) for the next k that will be computed */
        static seq_t pow2 = (seq_t)1;  /* 2^(1-1) = 1 for k = 1               */
        static idx_t pow2_k = 1;      /* current k for which pow2 holds 2^(k-1) */

        /* if we are just starting after a(0), pow2 corresponds to k=1 */
        if (pow2_k > computed) pow2_k = computed;

        for (idx_t k = computed; k <= n; ++k) {
            /* term = 2^(k-1) * a(k-1)                            */
            seq_t term = pow2 * cache[k - 1];

            /* convolution sum Σ_{i=1}^{k-1} a(i)*a(k-i)             */
            seq_t conv = (seq_t)0;
            for (idx_t i = 1; i < k; ++i) {
                conv += cache[i] * cache[k - i];
            }

            cache[k] = term - conv;

            /* prepare pow2 for next k (increase exponent by 1)      */
            pow2 <<= 1;
            ++pow2_k;
        }
        computed = n + 1;
    }

    return cache[n];
}
