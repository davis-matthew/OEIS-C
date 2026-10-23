/*
 * A122170  Number of primes p <= 2n such that p+n is also a prime.
 * https://oeis.org/A122170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p <= 2n such that p+n is also prime
 * Range:     u8 n <= 1889, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;

    /* limit for sieve: need primes up to 3n (p+n maximum) */
    idx_t limit_idx = 3 * n;
    size_t limit = (size_t)limit_idx;     /* assume limit fits into size_t */

    /* allocate sieve array (1 = prime, 0 = composite) */
    unsigned char *sieve = (unsigned char*)malloc(limit + 1);
    if (!sieve) return 0;                 /* out of memory: return 0 */

    /* initialise */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    /* Eratosthenes sieve */
    size_t max_root = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= max_root; ++p) {
        if (!sieve[p]) continue;
        for (size_t m = p * p; m <= limit; m += p) {
            sieve[m] = 0;
        }
    }

    /* count eligible primes */
    seq_t count = 0;
    idx_t two_n = 2 * n;
    for (idx_t p = 2; p <= two_n; ++p) {
        if (!sieve[(size_t)p]) continue;
        idx_t q = p + n;
        if (q <= limit_idx && sieve[(size_t)q]) {
            count++;
        }
    }

    free(sieve);
    return count;
}
