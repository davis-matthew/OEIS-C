/*
 * A161428  a(n) = A161424(n)/4.
 * https://oeis.org/A161428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A161428
 * Formula: a(n) = n + 3   for 1 ≤ n ≤ 5
 *            a(n) = prime(n‑1)   for n ≥ 6
 *
 * The sequence consists of the numbers 4,5,6,7,8 followed by all
 * primes starting with 11.  The function below computes the n‑th
 * term on demand, generating a sieve of Eratosthenes once and
 * caching the resulting primes.
 */

#include <stdint.h>
#include <stddef.h>

/* Basic parameters for the sieve.  Because the 10 000th prime is
   104 729, primes up to 200 000 are more than enough. */
#define SIEVE_LIMIT 200000

/* Static storage used by the sieve routine.  All are
   initialized to 0 at program start. */
static uint8_t  sieve[SIEVE_LIMIT + 1];   /* 0 = composite, 1 = potential prime */
static uint32_t primes[15000];           /* enough to hold > 10 000 primes */
static size_t   prime_cnt = 0;
static int      sieve_done = 0;

/* One‑time sieve of Eratosthenes; populates 'primes' and 'prime_cnt'. */
static void
init_sieve(void)
{
    if (sieve_done)
        return;

    /* Initialise the sieve array */
    for (int i = 0; i <= SIEVE_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Mark composites */
    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 0;

    /* Extract the primes into the 'primes' array */
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            primes[prime_cnt++] = (uint32_t)i;

    sieve_done = 1;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* The first five terms are simple (n + 3). */
    if (n <= 5)
        return (seq_t)(n + 3);

    /* Ensure that the sieve has been constructed. */
    init_sieve();

    /* For n ≥ 6, a(n) = the (n‑1)th prime.  Our array 'primes' is
       0‑based, so the index is (n‑2).  (n‑1)th prime = primes[n‑2]. */
    idx_t idx = n - 2;
    if ((size_t)idx < prime_cnt)
        return (seq_t)primes[(size_t)idx];

    /* If we somehow exceed the pre‑allocated range (unlikely for n ≤ 10 000)
       return 0.  The harness never requests such a value. */
    return (seq_t)0;
}
