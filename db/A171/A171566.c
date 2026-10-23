/*
 * A171566  Primes p such that 2*p-3 and 2*(2*p-3)-3 are primes (First member
 *          of a primes in a 2*p-3 double progression).
 * https://oeis.org/A171566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, isPrimeGeneric, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171566: primes p such that 2*p-3 and 2*(2*p-3)-3 are both prime. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"
#include "../utils/sieve_primes.h"

/* Returns the n-th term of A171566 (indexing starts at 1). */
seq_t a(idx_t n)
{
    /* static caches – preserved across calls */
    static idx_t *prime_list = NULL;   /* primes up to current limit */
    static size_t prime_count = 0;      /* number of primes stored */
    static idx_t prime_limit = 0;       /* maximum p considered so far */
    static idx_t prime_next_idx = 0;    /* next index in prime_list to test */

    static seq_t *seq_cache = NULL;    /* found terms of A171566 */
    static size_t seq_cap = 0;          /* capacity of seq_cache */
    static size_t seq_len = 0;          /* number of terms cached */

    /* Ensure we have at least n terms ready */
    while (seq_len < n) {
        /* If we have exhausted all primes we know, extend the sieve. */
        if (prime_next_idx >= prime_count) {
            /* growth strategy: double limit, starting at 1 000 000 */
            idx_t new_limit = prime_limit ? prime_limit << 1 : 1000000;
            /* Keep growing until the next prime we need is below the limit. */
            if (new_limit < 3) new_limit = 3;

            /* Rebuild the prime list up to new_limit. */
            idx_t *new_primes = NULL;
            size_t new_count = 0;
            sieve_primes(new_limit, &new_primes, &new_count);
            free(prime_list);
            prime_list  = new_primes;
            prime_count = new_count;
            prime_limit = new_limit;
            prime_next_idx = 0;           /* restart scanning from the first prime */
        }

        /* Scan primes for candidates. */
        while (prime_next_idx < prime_count) {
            idx_t p  = prime_list[prime_next_idx++];
            seq_t qp = (seq_t)p;
            seq_t q  = (seq_t)(qp * 2 - 3);
            if (!isPrimeGeneric(q)) continue;
            seq_t r  = (seq_t)(q * 2 - 3);
            if (!isPrimeGeneric(r)) continue;

            /* p satisfies the condition – store it. */
            grow_seq(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = qp;
            break; /* move on to the next term after storing. */
        }
    }

    /* Return the requested term (zero‑based indexing). */
    return seq_cache[n - 1];
}
