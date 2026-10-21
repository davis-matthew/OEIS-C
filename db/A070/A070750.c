/*
 * A070750  0 if n-th prime is even, 1 if n-th prime is == 1 (mod 4), and -1
 *          if n-th prime is == 3 (mod 4).
 * https://oeis.org/A070750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = 0 if the n‑th prime is even, 1 if it ≡ 1 (mod 4), -1 if
 *            it ≡ 3 (mod 4)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>   /* for log() in the upper‑bound estimate */

/* Static cache of primes.  prime_list stores primes in 0‑based order. */
static uint32_t *prime_list = NULL;
static size_t    prime_count = 0;   /* how many primes are stored */
static size_t    prime_cap   = 0;   /* capacity of prime_list */

/* Upper bound for the n‑th prime:  p_n ≤ n (ln n + ln ln n)  for n ≥ 6. */
static uint32_t prime_upper_bound(uint32_t n)
{
    if (n < 6)          /* first few primes < 15 */
        return 15;
    double dn   = (double)n;
    double buf  = dn * (log(dn) + log(log(dn)));
    return (uint32_t)(buf + 3.0);   /* small safety margin */
}

/* Ensure that at least n primes are available in prime_list. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;           /* already have enough */

    uint32_t target  = (uint32_t)n;
    uint32_t bound   = prime_upper_bound(target);

    /* Sieve of Eratosthenes */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(bound + 1), 1);
    if (!is_comp)
        return;           /* out‑of‑memory – caller will treat missing primes as error */

    /* Allocate or grow the prime list */
    if (prime_cap < target) {
        free(prime_list);
        prime_list = (uint32_t *)realloc(NULL, target * sizeof(uint32_t));
        if (!prime_list) {
            free(is_comp);
            return;
        }
        prime_cap = target;
    }

    prime_count = 0;
    for (uint32_t i = 2; i <= bound; ++i) {
        if (!is_comp[i]) {
            /* prime found */
            prime_list[prime_count++] = i;
            if (prime_count >= target)
                break;

            /* cross out multiples starting at i*i */
            if ((uint64_t)i * i <= bound) {
                for (uint32_t j = i * i; j <= bound; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but safe */

    /* The first prime is 2 (even) */
    if (n == 1)
        return (seq_t)0;

    ensure_primes(n);
    /* Now prime_list has at least n primes (0‑based). */
    uint32_t p = prime_list[n - 1];  /* n‑th prime */

    /* Even only for 2 (handled above).  For odd primes, see their residue mod 4. */
    if ((p % 4u) == 1u)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
