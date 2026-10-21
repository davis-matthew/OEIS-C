/*
 * A089882  Number of subsets of {1, ..., n} containing exactly one twin prime
 *          pair.
 * https://oeis.org/A089882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(A089881(n)/2) * 2^(n-A089881(n)), where A089881(n)
 *            is the number of integers ≤ n that belong to at least one twin
 *            prime pair. This function implements the formula directly, with
 *            a sieve to find primes up to n, a second pass to mark the twin
 *            elements, and then the required arithmetic
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate a sieve array */
    size_t sz = (size_t)(n + 1);
    bool *prime = (bool *)malloc(sz);
    if (!prime) return (seq_t)0;
    memset(prime, true, sz);
    if (sz > 0) prime[0] = false;
    if (sz > 1) prime[1] = false;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; (idx_t)i * (idx_t)i <= n; ++i) {
        if (prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                prime[j] = false;
        }
    }

    /* Mark elements that belong to at least one twin prime pair */
    bool *twin = (bool *)calloc(sz, sizeof(bool));
    if (!twin) { free(prime); return (seq_t)0; }

    for (size_t p = 2; p + 2 <= n; ++p) {
        if (prime[p] && prime[p + 2]) {
            twin[p] = true;
            twin[p + 2] = true;
        }
    }

    /* Count m = A089881(n) */
    uint64_t m = 0;
    for (size_t i = 2; i <= n; ++i)
        if (twin[i]) ++m;

    /* Compute ceil(m/2) as sequence type */
    seq_t half = (seq_t)((m + 1) / 2);

    /* Compute 2^(n - m) using repeated multiplication */
    idx_t exp = (idx_t)(n - m);
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;

    /* Final result */
    seq_t result = half * pow2;

    /* Clean up */
    free(prime);
    free(twin);
    return result;
}
