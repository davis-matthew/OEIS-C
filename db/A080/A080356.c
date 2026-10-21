/*
 * A080356  Number of twin primes between n and 2n: a(n) = number of j in
 *          range n <= j <= 2*n such that j and j+2 are primes.
 * https://oeis.org/A080356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Compute the number of twin primes between n and 2*n inclusive;
   uses a global sieve that grows as needed. */
static bool *prime_flags = NULL;  /* primality flags, indices 0 .. sieve_limit */
static idx_t sieve_limit = 0;     /* maximum number for which the sieve currently exists */

seq_t a(idx_t n)
{
    /* We need the sieve to cover up to 2*n + 2, because j + 2 may equal 2*n + 2. */
    idx_t needed = (n << 1) + 2;

    if (needed > sieve_limit) {
        /* Reallocate and rebuild the sieve. */
        free(prime_flags);
        prime_flags = (bool *)malloc((size_t)needed + 1);
        if (!prime_flags) {
            sieve_limit = 0;
            return (seq_t)0;          /* Out‑of‑memory fallback. */
        }

        /* Mark all numbers as prime initially. */
        for (idx_t i = 0; i <= needed; ++i) {
            prime_flags[(size_t)i] = true;
        }

        /* 0 and 1 are not prime. */
        if (needed >= 0) prime_flags[0] = false;
        if (needed >= 1) prime_flags[1] = false;

        /* Sieve of Eratosthenes. */
        for (idx_t p = 2; p * p <= needed; ++p) {
            if (prime_flags[(size_t)p]) {
                for (idx_t m = p * p; m <= needed; m += p) {
                    prime_flags[(size_t)m] = false;
                }
            }
        }

        sieve_limit = needed;
    }

    seq_t count = 0;
    idx_t upper = n + n;   /* 2*n */

    for (idx_t j = n; j <= upper; ++j) {
        if (prime_flags[(size_t)j] && prime_flags[(size_t)(j + 2)]) {
            count = count + 1;
        }
    }

    return count;
}
