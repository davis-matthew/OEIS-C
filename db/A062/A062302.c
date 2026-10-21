/*
 * A062302  Number of ways writing n-th prime as a sum of a prime and a
 *          nonprime.
 * https://oeis.org/A062302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write the n‑th prime as a sum * of a
 *            prime and a nonprime
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Prime table – grows as needed. */
static bool *prime_flag = NULL;   /* primality flags, 0‑based */
static idx_t *primes  = NULL;     /* list of primes in ascending order */
static size_t primes_cap   = 0;    /* allocated capacity */
static size_t primes_count = 0;    /* number of primes stored */
static idx_t  sieve_limit  = 0;    /* highest number already sieved */

/* Estimate an upper bound for the n‑th prime (Rosser 1938). */
static idx_t estimate_upper_prime(idx_t n)
{
    if (n < 6) return 15;          /* safe for small n */
    double dn   = (double)n;
    double upper = dn * (log(dn) + log(log(dn)));  /* double estimate */
    return (idx_t)(upper + 3.0);   /* add a safety margin */
}

/* Grow the sieve to at least new_limit and rebuild the prime list. */
static void grow_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    /* Resize primality flag array. */
    prime_flag = (bool *)realloc(prime_flag,
                                 (new_limit + 1) * sizeof(bool));
    if (!prime_flag)
        return;                    /* allocation failure – ignore */

    /* Initialise all entries to true; 0 and 1 are composite. */
    for (idx_t i = 0; i <= new_limit; ++i)
        prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;

    /* Classic Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                prime_flag[j] = false;
        }
    }

    /* Count primes to determine required capacity. */
    size_t needed = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (prime_flag[i]) ++needed;

    if (needed > primes_cap) {
        primes_cap = needed;
        primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        if (!primes)
            return;                 /* allocation failure – ignore */
    }

    /* Rebuild prime list. */
    primes_count = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (prime_flag[i]) {
            primes[primes_count++] = i;
        }

    sieve_limit = new_limit;
}

/* Ensure that at least n primes have been pre‑computed. */
static void ensure_primes_for_n(idx_t n)
{
    if (primes_count >= n)
        return;

    idx_t limit = estimate_upper_prime(n);
    grow_sieve(limit);
    while (primes_count < (size_t)n) {
        limit *= 2;
        grow_sieve(limit);
    }
}

/* Main function: compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* safety for offset 0 */

    ensure_primes_for_n(n);          /* primes[0..primes_count-1] ready */

    idx_t p = primes[n - 1];          /* n‑th prime (1‑based) */
    seq_t count = 0;                 /* result counter */

    /* Count primes q < p such that p – q is a nonprime > 0. */
    for (size_t i = 0; i < primes_count && primes[i] < p; ++i) {
        idx_t q = primes[i];
        idx_t r = p - q;             /* candidate nonprime part */
        if (r <= 1 || !prime_flag[r])
            ++count;
    }

    return count;
}
