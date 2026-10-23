/*
 * A154804  Number of ways to represent 2*n as the sum of two distinct primes
 *          (counting 1 as a prime).
 * https://oeis.org/A154804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n as a sum of two distinct
 *            primes, counting 1 as a prime. Computed by a sieve of
 *            Eratosthenes up to 2*n
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n==0, offset is undefined – return 0 */
    if (n == 0) return 0;

    static idx_t      current_limit = 0;   /* largest limit used so far */
    static uint64_t * primes         = NULL; /* array of primes >=2 */
    static size_t     prime_count    = 0;   /* number of primes stored */
    static bool *     prime_flags    = NULL; /* boolean array: prime_flags[i]==true -> i is prime */
    static size_t     prime_flags_sz = 0;   /* size of prime_flags array (current_limit+1) */

    idx_t limit = n + n;   /* 2*n */

    /* Rebuild sieve if a larger limit is requested */
    if (limit > current_limit) {
        /* free old prime_flags array */
        free(prime_flags);

        /* allocate new boolean array */
        prime_flags_sz = (size_t)limit + 1;
        prime_flags = (bool *)malloc(prime_flags_sz * sizeof(bool));
        if (!prime_flags) { free(primes); return 0; }

        /* initialise: all prime, except 0 and 1 */
        for (size_t i = 0; i < prime_flags_sz; ++i) prime_flags[i] = true;
        if (prime_flags_sz > 0) prime_flags[0] = false;
        if (prime_flags_sz > 1) prime_flags[1] = false;

        /* Eratosthenes sieve */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (prime_flags[i]) {
                for (idx_t j = i * i; j <= limit; j += i) prime_flags[j] = false;
            }
        }

        /* count primes and collect them into array */
        size_t new_cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) if (prime_flags[i]) ++new_cnt;

        uint64_t * new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        if (!new_primes) { free(prime_flags); free(primes); return 0; }

        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i) if (prime_flags[i]) new_primes[idx++] = (uint64_t)i;

        /* free old primes array and replace */
        free(primes);
        primes = new_primes;
        prime_count = new_cnt;
        current_limit = limit;
    }

    /* Count pairs of distinct primes (p < q, p+q = 2*n) */
    seq_t cnt = 0;
    for (size_t i = 0; i < prime_count && primes[i] <= limit / 2; ++i) {
        idx_t p = primes[i];
        idx_t q = limit - p;
        if (p < q && prime_flags[q]) ++cnt;
    }

    /* Count the pair (1, limit-1) if limit-1 is a prime (1 is considered prime) */
    if (limit > 2 && prime_flags[limit - 1]) ++cnt;

    return cnt;
}
