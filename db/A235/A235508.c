/*
 * A235508  Number of ways to write 2*n = p + q with q > 0 such that p,
 *          p*(p+1) - prime(p) and prime(q) - q + 1 are all prime.
 * https://oeis.org/A235508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to 2*n = p + q with q>0, such that *
 *            p is prime, * p*(p+1) - prime(p) is prime, * prime(q) - q + 1 is
 *            prime
 * Range:     u8 n <= 9, u16 n <= 399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_small.h"   // bool is_prime_small(uint64_t)

static size_t flag_limit = 0;               /* size of prime_flags array */
static bool *prime_flags = NULL;            /* sieve array, true = prime */
static uint32_t *primes = NULL;             /* primes list, 1-based indexing via p-1 */
static size_t primes_count = 0;             /* number of primes stored */

/* Build a sieve up to a limit that contains at least `index_needed` primes. */
static void build_sieve_for_index(size_t index_needed) {
    if (primes_count >= index_needed)
        return;

    double d = (double)index_needed;
    size_t limit = (size_t)(d * (log(d) + log(log(d))) + 10.0);
    if (limit < 100)
        limit = 100;

    while (1) {
        bool *flags = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!flags)
            exit(1);                     /* allocation failure */

        flags[0] = false;
        if (limit >= 1)
            flags[1] = false;
        for (size_t i = 2; i <= limit; ++i)
            flags[i] = true;

        for (size_t i = 2; (size_t)i * i <= limit; ++i)
            if (flags[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    flags[j] = false;
            }

        size_t count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (flags[i])
                ++count;

        if (count >= index_needed) {
            free(prime_flags);
            free(primes);
            prime_flags = flags;
            primes = (uint32_t *)malloc(count * sizeof(uint32_t));
            if (!primes)
                exit(1);
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (flags[i])
                    primes[idx++] = (uint32_t)i;
            primes_count = count;
            flag_limit   = limit + 1;     /* array length */
            return;
        }

        free(flags);
        limit *= 2;                      /* increase limit and retry */
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* ensure we have enough primes for indices up to 2*n */
    build_sieve_for_index((size_t)(2 * n));

    idx_t two_n = 2 * n;
    seq_t count = 0;

    for (idx_t p = 1; p < two_n; ++p) {
        if (p >= flag_limit || !prime_flags[p])
            continue;                   /* p is not prime */

        /* p is prime; check first condition */
        seq_t t1 = (seq_t)p * (p + 1) - (seq_t)primes[p - 1];
        if (!is_prime_small((uint64_t)t1))
            continue;

        idx_t q = two_n - p;
        if (q == 0)
            continue;                   /* q must be > 0 */

        /* check second condition */
        seq_t t2 = (seq_t)primes[q - 1] - q + 1;
        if (!is_prime_small((uint64_t)t2))
            continue;

        ++count;                       /* a valid pair found */
    }

    return count;
}
