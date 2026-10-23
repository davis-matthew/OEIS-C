/*
 * A142906  Primes congruent to 31 mod 63.
 * https://oeis.org/A142906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 31 (mod 63) (a(1)=31)
 * Range:     u8 n <= 2, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t curr_limit = 0;          /* last sieved limit */
static uint8_t *sieve = NULL;        /* sieve array, 0 = prime, 1 = composite */
static uint64_t *residue_primes = NULL; /* primes ≡ 31 mod 63, 0‑based */
static size_t residue_count = 0;     /* number of stored primes */

/* returns the nth such prime; 1‑based index */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n < 1 */

    /* keep building until we have at least n primes of the required residue */
    while (residue_count < (size_t)n) {

        /* choose new limit: double each time, start at 1 000 */
        idx_t new_limit = curr_limit ? curr_limit * 2 : 1000;
        if (new_limit < 1000)
            new_limit = 1000;

        /* allocate sieve */
        size_t sieve_len = (size_t)new_limit + 1;
        uint8_t *tmp_sieve = (uint8_t *)realloc(sieve, sieve_len);
        if (!tmp_sieve)
            return (seq_t)0;
        sieve = tmp_sieve;
        memset(sieve, 0, sieve_len);          /* assume all prime initially */

        /* simple Eratosthenes sieve up to new_limit */
        for (idx_t p = 2; p * p <= new_limit; ++p) {
            if (sieve[p] == 0) {
                for (idx_t m = p * p; m <= new_limit; m += p)
                    sieve[m] = 1;
            }
        }

        /* rebuild the array of primes in the needed residue class */
        /* free previous array if any */
        if (residue_primes)
            free(residue_primes);

        /* rough estimate of count: new_limit / 10 + 10  */
        size_t est_cap = new_limit / 10 + 10;
        residue_primes = (uint64_t *)malloc(est_cap * sizeof(uint64_t));
        if (!residue_primes)
            return (seq_t)0;
        residue_count = 0;

        for (idx_t i = 2; i <= new_limit; ++i) {
            if (sieve[i] == 0 && (i % 63) == 31) {
                if (residue_count == est_cap) {
                    est_cap *= 2;
                    uint64_t *tmp = (uint64_t *)
                        realloc(residue_primes, est_cap * sizeof(uint64_t));
                    if (!tmp)
                        return (seq_t)0;
                    residue_primes = tmp;
                }
                residue_primes[residue_count++] = (uint64_t)i;
            }
        }

        curr_limit = new_limit;   /* update current sieve limit */
    }

    /* return the requested prime (n is 1‑based, array is 0‑based) */
    return (seq_t)(residue_primes[n - 1]);
}
