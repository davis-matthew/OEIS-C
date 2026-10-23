/*
 * A167623  a(n) = n^3 mod (n-th prime squared).
 * https://oeis.org/A167623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod (p_n^2) where p_n is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1651, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static prime cache */
static uint64_t *prime_array = NULL;   /* list of primes */
static size_t   prime_count = 0;       /* number of primes stored */
static uint64_t prime_limit  = 0;      /* maximum prime generated */

/* Ensure we have at least 'need' primes stored in prime_array.
   Recomputes the prime list only when necessary. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need) return;   /* already enough */

    /* Rough upper bound for the n-th prime: p_n < n (log n + log log n) for n >= 6 */
    double dn    = (double)need;
    double est   = dn * (log(dn) + log(log(dn))) + 3.0;
    if (est < 100.0) est = 100.0;
    uint64_t new_limit = (uint64_t)est + 10;

    /* Make sure the limit never goes down */
    if (new_limit <= prime_limit) new_limit = prime_limit * 2 + 1;

    /* Sieve of Eratosthenes up to new_limit */
    uint8_t *is_comp = (uint8_t *)calloc(new_limit + 1, 1);
    if (!is_comp) return;           /* allocation failure: give up */

    for (uint64_t p = 2; p * p <= new_limit; ++p) {
        if (!is_comp[p]) {
            for (uint64_t m = p * p; m <= new_limit; m += p)
                is_comp[m] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate new prime array */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_comp);
        return;
    }

    /* Store primes */
    cnt = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) new_primes[cnt++] = i;

    free(is_comp);
    free(prime_array);

    prime_array = new_primes;
    prime_count = cnt;
    prime_limit = new_limit;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return safe 0 */

    /* Make sure we have the n-th prime */
    ensure_primes(n);

    /* Compute n^3 mod (p_n^2) using seq_t arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t p    = (seq_t)prime_array[n - 1];
    seq_t mod  = p * p;
    seq_t cube = nn * nn * nn;

    return cube % mod;
}
