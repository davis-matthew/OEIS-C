/*
 * A172103  Partial sums of A167020 where A167020(n)=1 iff 6*n-1 is prime.
 * https://oeis.org/A172103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} [is_prime(6*k-1)] (0‑based boolean sum)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n==0 is undefined but we return 0 */
    if (n == 0) return (seq_t)0;

    /* static cache: a(i) stored at cache[i-1] for i>=1 */
    static seq_t *cache = NULL;
    static idx_t cache_N = 0;   /* largest n that has been computed */

    if (n > cache_N) {
        /* grow cache to hold terms up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failure – unlikely */

        cache = tmp;
        /* current sum of primes seen up to cache_N */
        seq_t sum = (cache_N > 0) ? cache[cache_N - 1] : (seq_t)0;

        for (idx_t i = cache_N + 1; i <= n; ++i) {
            idx_t m = 6 * i - 1;            /* number to test for primality */
            sum += (seq_t)(is_prime(m) ? 1 : 0);
            cache[i - 1] = sum;             /* store a(i) */
        }
        cache_N = n;
    }

    return cache[n - 1];
}
