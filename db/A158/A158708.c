/*
 * A158708  Primes p such that p + floor(p/2) is prime.
 * https://oeis.org/A158708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p + floor(p/2) is prime
 * Range:     u8 n <= 10, u16 n <= 504, u32 n <= 505, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* helpers provided by the test harness */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0, return 0 as a sentinel */

    idx_t count = 0;          /* how many terms we have found */
    uint64_t candidate = 1;   /* start before the first prime (2) */

    while (count < n) {
        /* get the next prime number */
        candidate = next_prime(candidate);

        /* compute q = p + floor(p/2). For p odd, floor(p/2) = (p-1)/2.
           For p=2, floor(p/2)=1, so q = 3. The formula (3*p-1)/2 works for
           all primes (including 2) because  (3*2-1)/2 = 5, which is the
           correct q value to test for primality. */
        uint64_t q = (3ULL * candidate - 1ULL) / 2ULL;

        if (is_prime_big((seq_t)q)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
    }

    /* unreachable, but keeps the compiler happy */
    return 0;
}
