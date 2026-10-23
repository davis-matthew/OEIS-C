/*
 * A104293  a(n) = prime((prime(n)-1)/2).
 * https://oeis.org/A104293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime((prime(n)-1)/2) Computes prime numbers by a simple
 *            dynamic sieve and uses them to evaluate the sequence for any n ≥
 *            2. The function is safe for seq_t being an arbitrary‑precision
 *            integer class (the particular types are provided by the build
 *            system)
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

/* static storage for the list of known primes, 1‑based */
static seq_t *primes = NULL;          /* primes[0] = 2, primes[1] = 3, ... */
static idx_t  prime_count = 0;        /* number of primes currently stored */

/* sieve buffer, 0 means composite, 1 means not prime */
static uint8_t *sieve = NULL;

/* ------------------------------------------------------------------ */
/* ensure_primes(need)   : guarantee that primes array contains at least
   need primes (need ≥ 1).  The function expands the array if
   necessary.  It uses a simple Sieve of Eratosthenes up to an
   upper bound returned by estimate_prime_upper_bound(need). */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return; /* already enough */

    /* find an upper bound for the nth prime and allocate the sieve */
    idx_t limit = estimate_prime_upper_bound(need);
    if (sieve) {
        free((void *)sieve);
        sieve = NULL;
    }
    sieve = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!sieve)
        return;                      /* allocation failure – abort */

    sieve[0] = 1;
    sieve[1] = 1;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* count primes up to the limit */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++count;

    /* allocate the primes array */
    if (primes) {
        free((void *)primes);
        primes = NULL;
    }
    primes = (seq_t *)malloc((size_t)count * sizeof(seq_t));
    if (!primes) {
        free((void *)sieve);
        sieve = NULL;
        return;                      /* allocation failure – abort */
    }

    /* store the primes in the array */
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            primes[idx++] = (seq_t)i;
    prime_count = count;
}

/* ------------------------------------------------------------------ */
/* seq_t a(idx_t n)   : return the nth term of A104293, with offset 2.
   For n < 2 the function returns 0, but the behaviour for such
   indices is irrelevant. */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for indices below the offset */

    /* prime(n) */
    ensure_primes(n);
    seq_t pn = primes[n - 1];     /* primes array is 0‑based */

    /* compute (pn - 1)/2 as an index (1‑based) for the second prime */
    idx_t mid = (idx_t)((uint64_t)pn - 1) / 2;

    /* prime(mid) */
    ensure_primes(mid);
    return primes[mid - 1];
}
