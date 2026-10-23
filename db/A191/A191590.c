/*
 * A191590  a(n) is the genus of the modular curve associated to the principal
 *          congruence subgroup of level p(n), where p(n) is the n-th prime
 *          number.
 * https://oeis.org/A191590
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)+2)*(prime(n)-3)*(prime(n)-5)/24
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for primes: primes[i] is the (i+1)-th prime */
static uint64_t *primes = NULL;       /* array of primes (0‑based) */
static size_t   primes_count = 0;     /* number of primes stored      */
static size_t   primes_capacity = 0;  /* allocated capacity           */

/* ensure that at least n primes are available (n >= 1) */
static void ensure_primes(idx_t n)
{
    if (n <= primes_count)
        return;

    /* initial capacity */
    if (primes_capacity == 0) {
        primes_capacity = 16;
        primes = (uint64_t*)malloc(primes_capacity * sizeof(uint64_t));
    }

    while (primes_count < n) {
        uint64_t candidate;
        /* pick next odd candidate */
        if (primes_count == 0)
            candidate = 2;
        else if (primes_count == 1)
            candidate = 3;
        else
            candidate = primes[primes_count - 1] + 2; /* skip evens */

        /* find next prime by trial division */
        while (1) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_count; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;          /* candidate is prime */
            candidate += 2;     /* try next odd number */
        }

        /* ensure capacity before storing */
        if (primes_count == primes_capacity) {
            primes_capacity *= 2;
            primes = (uint64_t*)realloc(primes,
                                        primes_capacity * sizeof(uint64_t));
        }
        primes[primes_count++] = candidate;  /* store the new prime */
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* n is 1‑based index of the prime in OEIS */
    size_t idx = (size_t)n;            /* safe for n up to 10⁴ and beyond */
    ensure_primes(idx);                /* primes[0 .. idx-1] are ready */

    seq_t p = (seq_t)primes[idx - 1];   /* prime(n) */
    seq_t term = (p + 2) * (p - 3) * (p - 5);
    return term / 24;
}
