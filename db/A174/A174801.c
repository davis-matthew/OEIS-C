/*
 * A174801  Prime(n)+1 is prime or semiprime.
 * https://oeis.org/A174801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer m such that prime(m)+1
   is prime or semiprime.  The implementation searches m in ascending
   order using an on‑demand sieve of Eratosthenes and trial‑division
   primality tests. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Prime sieve and cache ------------------------------------------- */

static uint64_t *prime_list = NULL;   /* primes (0‑based indexing) */
static size_t   prime_count = 0;      /* number of primes stored      */
static uint8_t *sieve       = NULL;   /* sieve array                  */
static size_t   sieve_limit = 0;      /* current sieve upperbound     */

/* Ensure that prime_list contains at least the first `required` primes. */
static void ensure_sieve(size_t required)
{
    if (required <= prime_count)
        return;

    /* Choose a new sieve limit that is safely larger than the required
       number of primes.  This simple heuristic overshoots enough for
       the intended range (n up to ~10000). */
    size_t new_limit = required * 20 + 100;
    if (new_limit <= sieve_limit)
        new_limit = sieve_limit * 2 + 100;

    uint8_t *new_sieve = (uint8_t *)malloc(new_limit + 1);
    if (!new_sieve) abort();
    for (size_t i = 0; i <= new_limit; ++i)
        new_sieve[i] = 1;
    new_sieve[0] = new_sieve[1] = 0;

    for (size_t i = 2; i * i <= new_limit; ++i)
        if (new_sieve[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = 0;

    /* Count primes in the new sieve. */
    size_t count = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i]) ++count;

    uint64_t *new_primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!new_primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_sieve[i]) new_primes[idx++] = i;

    /* Replace old data.  free(NULL) is safe. */
    free(sieve);
    free(prime_list);
    sieve = new_sieve;
    sieve_limit = new_limit;
    prime_list = new_primes;
    prime_count = count;
}

/* --- Prime or semiprime test ------------------------------------------ */

static bool prime_or_semiprime(uint64_t val)
{
    /* Quick prime test: trial division using known primes. */
    if (val < 2)
        return false;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= val; ++i) {
        uint64_t p = prime_list[i];
        if (val % p == 0) {
            /* val is divisible by p; now count total factors. */
            uint64_t x = val;
            int cnt = 0;
            while (x % p == 0) {
                ++cnt;
                x /= p;
                if (cnt > 2)
                    return false;
            }
            if (x > 1) {
                /* remaining factor must be prime */
                ++cnt;
                if (cnt > 2)
                    return false;
                for (size_t j = 0; j < prime_count && prime_list[j] * prime_list[j] <= x; ++j)
                    if (x % prime_list[j] == 0)
                        return false;
            }
            return cnt == 1 || cnt == 2;
        }
    }
    /* If we reach here, val itself is prime. */
    return true;
}

/* --- Sequence computation --------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* undefined for n < 1 */

    uint64_t m = 1;         /* candidate integer m */
    idx_t    count = 0;     /* how many terms found so far */

    while (count < n) {
        /* Ensure we have enough primes to access prime(m). */
        ensure_sieve((size_t)m);

        uint64_t p = prime_list[m - 1];  /* prime(m) */

        if (prime_or_semiprime(p + 1)) {
            ++count;
            if (count == n)
                return (seq_t)m;      /* found the n-th term */
        }
        ++m;
    }
    /* Unreachable, but keep compiler happy. */
    return 0;
}
