/*
 * A076240  Remainder when 2nd order prime pp(n) = A006450(n) is divided by
 *          n-th prime = A000040(n).
 * https://oeis.org/A076240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = prime(prime(n)) mod prime(n)
 * Range:     u8 n <= 16, u16 n <= 1238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to 2 000 000. */
/* For n up to ~100 000 this provides enough primes to compute */
#include <stdbool.h>
#include <stddef.h>

/* Maximum prime value pre‑indexed in the sieve.  */
/* Must be > the (n‑th prime) for all n used in the tests. */
#define SIEVE_MAX 2000000U

/* Static storage for the sieve and the prime list. */
static bool sieve_array[SIEVE_MAX + 1] = {0};    /* 0 → potentially prime, 1 → composite */
static seq_t primes_array[200000];              /* 200 000 > number of primes ≤ 2 000 000 */
static size_t prime_count = 0;
static bool primes_initialised = false;

/* ----------------------------------------------------------- */
/* Helper: initialise the prime tables once. */
static void init_primes(void)
{
    if (primes_initialised) return;

    /* Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= SIEVE_MAX; ++p)
    {
        if (!sieve_array[p])
        {
            for (idx_t q = p * p; q <= SIEVE_MAX; q += p)
                sieve_array[q] = true;
        }
    }

    /* Extract primes into the array. */
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
    {
        if (!sieve_array[i])
        {
            if (prime_count < 200000)   /* safety against overflow */
                primes_array[prime_count++] = (seq_t)i;
        }
    }
    primes_initialised = true;
}

/* ----------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. */
    if (n < 1) return 0;

    init_primes();

    /* Ensure we have at least n primes. */
    if (prime_count < (size_t)n) return 0;

    /* Retrieve prime(n).  primes_array is 0‑based. */
    seq_t pn = primes_array[(size_t)n - 1];

    /* Index for prime(pn).  pn is certainly < SIEVE_MAX and > 0. */
    idx_t idx_pn = (idx_t)pn;
    if (idx_pn < 1 || idx_pn > prime_count) return 0;

    seq_t pp = primes_array[(size_t)idx_pn - 1];

    return pp % pn;
}
