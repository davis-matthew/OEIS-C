/*
 * A168245  prime(prime(n+1))-2*prime(n).
 * https://oeis.org/A168245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n+1)) - 2 * prime(n) (offset = 1)
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime generation parameters */
#define PRIMES_LIMIT      2000000U   /* sieve up to this value */
#define MAX_PRIME_COUNT   200000U   /* maximum number of primes stored */

/* Prime storage */
static uint32_t primes[MAX_PRIME_COUNT];
static size_t   prime_count = 0;
static int      primes_inited = 0;

/* One‑time sieve of Eratosthenes */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = 1;

    /* Allocate boolean array for composite markers */
    char *is_comp = (char *)calloc(PRIMES_LIMIT + 1U, 1U);
    if (!is_comp) abort();          /* out of memory */

    /* Perform the sieve */
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count < MAX_PRIME_COUNT)
                primes[prime_count++] = (uint32_t)i;
            if (i * i <= PRIMES_LIMIT) {
                for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; ignore arguments < 1 */
    if (n < 1) return (seq_t)0;

    /* Ensure primes are available */
    init_primes();

    /* We need prime(n), prime(n+1), and prime(prime(n+1))          */
    /* All three indices must be ≤ prime_count.                  */
    if ((size_t)n >= prime_count)                return (seq_t)0;
    if ((size_t)(n + 1) >= prime_count)          return (seq_t)0;

    /* Retrieve prime(n) and prime(n+1) as uint32_t values  */
    uint32_t p_n       = primes[(size_t)(n - 1)];      /* prime(n)   */
    uint32_t p_n_plus1 = primes[(size_t)(n)];        /* prime(n+1) */

    /* prime(n+1) is used as an index for the next prime.  */
    if ((size_t)(p_n_plus1) >= prime_count)         return (seq_t)0;

    /* prime(prime(n+1)) */
    uint32_t p_pp_n_plus1 = primes[(size_t)(p_n_plus1) - 1U];

    /* Compute the term in the required output type */
    return (seq_t)p_pp_n_plus1 - (seq_t)(2U * p_n);
}
