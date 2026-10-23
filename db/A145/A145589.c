/*
 * A145589  a(n) = number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^11.
 * https://oeis.org/A145589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simulate Eratosthenes' sieve for the range 2 … 2^11 (2048).
   a(n) is the number of new composite numbers removed when the n‑th
   prime is used as the sieving prime.  The function uses a static
   sieve state so that subsequent calls are efficient. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Limit of the sieve: 2^11 = 2048 */
    const idx_t LIMIT = ((idx_t)1) << 11;          /* 2048 */

    /* Static pre‑computed data */
    static bool composite[2049]; /* composite[i] == true if i is composite */
    static bool removed[2049];   /* removed[i] == true if i has been removed */
    static unsigned primes[2048]; /* list of primes up to LIMIT */
    static size_t prime_count = 0; /* number of primes found */
    static bool initialized = false;

    /* Static arrays to store the counts for each step */
    static seq_t removals[2048];
    static size_t steps_done = 0; /* number of steps already computed */

    /* Initialise sieve once */
    if (!initialized) {
        /* Base cases */
        for (size_t i = 0; i <= LIMIT; ++i) {
            composite[i] = false;
            removed[i]   = false;
        }
        composite[0] = composite[1] = true;

        /* Classic Sieve of Eratosthenes to find primes up to LIMIT */
        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (!composite[p]) {
                for (idx_t j = p * p; j <= LIMIT; j += p) {
                    composite[j] = true;
                }
            }
        }

        /* Collect primes */
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (!composite[i]) {
                primes[prime_count++] = (unsigned)i;
            }
        }
        initialized = true;
    }

    /* Compute additional steps if needed */
    while (steps_done < n && steps_done < prime_count) {
        seq_t cnt = 0;
        unsigned p = primes[steps_done];

        /* Remove all multiples of p that are not already removed. p itself is not removed. */
        for (idx_t m = p * 2; m <= LIMIT; m += p) {
            if (!removed[m]) {
                removed[m] = true;
                cnt++;
            }
        }

        removals[steps_done] = cnt;
        ++steps_done;
    }

    if (n > 0 && (size_t)(n - 1) < steps_done) {
        return removals[n - 1];
    }
    /* If n is beyond the number of primes, no further removals occur. */
    return (seq_t)0;
}
