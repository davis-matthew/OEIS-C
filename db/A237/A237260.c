/*
 * A237260  Least positive integer m < n with prime(prime(m)) + 2 and
 *          prime(n-m) + 2 both prime, or 0 if such a number m does not exist.
 * https://oeis.org/A237260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 6631, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least positive integer m < n such that
   prime(prime(m)) + 2 and prime(n–m) + 2 are both prime,
   or 0 if no such m exists. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper prototypes available in the build */
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"

/* static sieve up to a fixed limit (enough for n up to at least 100 000) */
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;
    /* Number of primes below 2 000 000 is 148 933, enough for the
       required indices (prime(100 000) ≈ 1 299 709). */
    const idx_t limit = 2000000;
    sieve_primes(limit, &primes, &prime_count);
    primes_inited = true;
}

/* seq_t a(idx_t n) implementation */
seq_t a(idx_t n)
{
    init_primes();

    if (n <= 2)
        return 0;                   /* a(1) = a(2) = 0 */

    /* search for the smallest m < n */
    for (idx_t m = 1; m < n; ++m) {
        /* pm = prime(m) */
        if ((size_t)m > prime_count)
            break;                  /* should not happen for n within limits */
        uint64_t pm = primes[(size_t)m - 1];

        /* ppmp = prime(prime(m)) = prime(pm) */
        if (pm == 0 || (size_t)pm == 0 || (size_t)pm > prime_count)
            continue;               /* safety check */
        uint64_t ppmp = primes[(size_t)pm - 1];

        /* pn = prime(n‑m) */
        uint64_t pn = primes[(size_t)(n - m) - 1];

        /* test primality of ppmp + 2 and pn + 2 */
        seq_t test1 = (seq_t)ppmp + 2;
        seq_t test2 = (seq_t)pn + 2;

        if (is_prime((idx_t)test1) && is_prime((idx_t)test2))
            return (seq_t)m;
    }
    return 0;
}
