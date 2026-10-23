/*
 * A175220  The fifth nonprimes after the primes.
 * https://oeis.org/A175220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the 5th composite after the nth prime (composites are
 *            counted as numbers >1 that are not prime)
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Global storage for the list of primes */
static uint64_t *primes = NULL;      /* array of primes (minimal type) */
static size_t primes_cnt = 0;        /* number of primes stored */

/* Ensure that at least `required` primes are available.
   If not, estimate a bound, rebuild the array, and repeat until sufficient. */
static void ensure_primes_upto(idx_t required)
{
    if (primes_cnt >= (size_t)required)
        return;                   /* already have enough */

    /* free old list if it exists */
    if (primes != NULL) {
        free(primes);
        primes = NULL;
    }

    /* rough upper bound for the n-th prime: p_n < n*(20) + 1000
       for the required n (≤~10⁴ this is far above the real value).    */
    idx_t limit = required * 20 + 1000;
    while (1) {
        build_primes_upto(limit, &primes, &primes_cnt);
        if (primes_cnt >= (size_t)required)
            break;               /* enough primes */
        /* bound too low – double it and try again */
        limit *= 2;
        if (primes != NULL) {
            free(primes);
            primes = NULL;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for the OEIS, but never used */

    /* Make sure we have the first n primes available */
    ensure_primes_upto(n);

    /* The nth prime is stored at primes[n-1] (zero‑based index) */
    uint64_t p = primes[(size_t)n - 1];

    uint64_t candidate = p + 1;
    int found = 0;

    while (found < 5) {
        if (!is_prime((idx_t)candidate)) {
            ++found;
            if (found == 5)
                break;
        }
        ++candidate;
    }

    return (seq_t)candidate;
}
