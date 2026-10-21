/*
 * A075069  Product of prime(n) consecutive numbers starting from prime(n).
 * https://oeis.org/A075069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2p(n)−1)!/(p(n)−1)! = Π_{k=0}^{p(n)−1} (p(n)+k), where
 *            p(n) is the n‑th prime (OEIS A075069)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Return the n‑th prime (n >= 1).  A simple incremental sieve is used. */
static seq_t nth_prime(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, but never called */

    /* Estimate an upper bound for the n‑th prime:
       p_n < n(ln n + ln ln n) for n ≥ 6. */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 3.0);
    }

    while (1) {
        /* Allocate a sieve array of size limit+1.  1 means prime candidate. */
        char *sieve = (char *)malloc((size_t)limit + 1);
        if (!sieve)
            return 0; /* out of memory – unlikely in the test harness */

        /* Initialise the sieve. */
        for (size_t i = 0; i <= (size_t)limit; ++i)
            sieve[i] = 1;
        sieve[0] = sieve[1] = 0;

        /* Eratosthenes sieve. */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 0;
            }
        }

        /* Count primes until we reach the n‑th. */
        idx_t count = 0;
        uint64_t p = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (sieve[i]) {
                ++count;
                if (count == n) {
                    p = i;
                    break;
                }
            }
        }

        free(sieve);

        if (count >= n)
            return (seq_t)p;          /* cast to seq_t for the caller */

        /* If not enough primes, double the bound and repeat. */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    seq_t prime = nth_prime(n);             /* p(n) as seq_t */
    idx_t p = (idx_t)prime;                 /* same numeric value in idx_t */

    seq_t prod = 1;
    for (idx_t i = 0; i < p; ++i)
        prod = prod * (prime + (seq_t)i);

    return prod;
}
