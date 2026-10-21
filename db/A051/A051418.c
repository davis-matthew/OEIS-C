/*
 * A051418  Square of LCM of {1, 2, ..., n}.
 * https://oeis.org/A051418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = lcm(1,2,…,n)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least common multiple of 1…n by prime powers,
   then returns its square.  The implementation is O(n log log n)
   and works for any n ≥ 1. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return 1;            /* offset starts at 1 */

    /* Sieve to find primes up to n */
    idx_t size = (idx_t)(n + 1);
    char *isprime = (char *)malloc((size_t)size);
    if (!isprime) return 0;          /* allocation failure */

    memset(isprime, 1, size);
    isprime[0] = isprime[1] = 0;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                isprime[j] = 0;
        }
    }

    /* Compute lcm = product of p^e for all primes p ≤ n with e = floor(log_p n) */
    seq_t lcm = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!isprime[i]) continue;
        idx_t prime = i;
        idx_t power = prime;
        /* find highest power of prime that is ≤ n */
        while (power <= n / prime)
            power *= prime;
        lcm *= (seq_t)power;
    }

    free(isprime);

    return lcm * lcm;
}
