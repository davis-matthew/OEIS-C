/*
 * A142255  Primes congruent to 6 mod 43.
 * https://oeis.org/A142255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A142255: primes congruent to 6 (mod 43) */
/* The function searches for the n‑th prime p such that p ≡ 6 (mod 43).  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Computes the n‑th prime congruent to 6 mod 43.  */
seq_t a(idx_t n)
{
    /* Basic checks: OEIS offset starts at 1.  */
    if (n == 0) return 0;

    /* An upper bound that is safely above the n‑th such prime.
       For n ≤ 10000 a bound of n * 43 * 15 + 100 exceeds the required value. */
    idx_t bound = n * 43 * 15 + 100;

    /* Allocate a simple sieve array.  */
    size_t sz = (size_t)(bound + 1);
    bool *sieve = (bool *)malloc(sz);
    if (!sieve) return 0;                /* allocation failure */

    /* Initialise all entries to false (prime).  */
    for (size_t i = 0; i < sz; ++i) sieve[i] = false;

    /* Eratosthenes sieve.  */
    for (idx_t i = 2; i <= bound / i; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= bound; j += i) {
                sieve[j] = true;         /* mark composite */
            }
        }
    }

    /* Find the n‑th prime ≡ 6 (mod 43).  */
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t x = 6; x <= bound; x += 43) {
        if (!sieve[x]) {                 /* prime found */
            ++count;
            if (count == n) {
                result = (seq_t)x;
                break;
            }
        }
    }

    free(sieve);
    return result;
}
