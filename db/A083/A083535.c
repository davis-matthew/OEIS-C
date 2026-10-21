/*
 * A083535  A version of the Chebyshev function theta(n): a(n) =
 *          ceiling(Sum_{primes p <= n } log(p)).
 * https://oeis.org/A083535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( Σ_{p ≤ n} log(p) ) , a(0) = a(1) = 0
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* No primes ≤ 1 */
    if (n < 2) return (seq_t)0;

    /* Allocate a sieve array of size n + 1 */
    size_t sz = (size_t)n + 1;
    unsigned char *prime = (unsigned char *)malloc(sz);
    if (!prime) return (seq_t)0;          /* allocation failure – unlikely */

    /* Initialise */
    for (size_t i = 0; i < sz; ++i) prime[i] = 1;
    prime[0] = prime[1] = 0;

    /* Simple Eratosthenes sieve */
    for (size_t p = 2; p * p <= n; ++p) {
        if (prime[p]) {
            for (size_t j = p * p; j <= n; j += p) prime[j] = 0;
        }
    }

    /* Accumulate the sum of logarithms (using long double for precision) */
    long double sum = 0.0L;
    for (size_t i = 2; i <= n; ++i)
        if (prime[i]) sum += log((long double)i);

    free(prime);

    /* The sum is never an integer;  ceil(sum) = floor(sum) + 1.
       long double precision is sufficient for the sizes required.   */
    unsigned long long floor_val = (unsigned long long)sum;
    return (seq_t)(floor_val + 1);
}
