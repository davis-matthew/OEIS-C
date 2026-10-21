/*
 * A079883  a(1) = 1; a(n) = prime(n) - prime(n-1)* a(n-1) if n > 1.
 * https://oeis.org/A079883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = prime(n) - prime(n-1) * a(n-1) (n > 1)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* Undefined for n < 1, value irrelevant */

#ifdef __cplusplus
    /* Cache previously computed terms using std::vector for efficiency */
    static std::vector<seq_t> cache;
    static idx_t computed = 0;   /* index of largest term computed */

    if (computed == 0) {
        /* Initialise cache with a(1) */
        cache.resize(2);          /* indices 0 and 1 */
        cache[1] = (seq_t)1;
        computed = 1;
    }

    if (n > computed) {
        cache.resize((size_t)n + 1);  /* Ensure space for indices up to n */

        for (idx_t k = computed + 1; k <= n; ++k) {
            uint32_t pk  = get_prime(k);
            uint32_t pk1 = get_prime(k - 1);
            cache[k] = (seq_t)pk - (seq_t)pk1 * cache[k - 1];
        }
        computed = n;
    }

    return cache[n];
#else
    /* Plain iterative computation for C builds (no caching) */
    seq_t value = (seq_t)1;  /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        uint32_t pk  = get_prime(k);
        uint32_t pk1 = get_prime(k - 1);
        value = (seq_t)pk - (seq_t)pk1 * value;
    }

    return value;
#endif
}
