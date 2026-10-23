/*
 * A040987  Primes p such that x^5 = 21 has no solution mod p.
 * https://oeis.org/A040987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A040987: primes p such that x^5 = 21 has no solution mod p.
 * The sequence consists of primes p with p ≡ 1 (mod 5) for which
 * 21 is not a 5th power residue modulo p.  For such a prime p the
 * congruence x^5 ≡ 21 (mod p) has no solution; for all other primes
 * it has a solution.
 *
 * Formula: is read by the code as described above.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/build_primes.h"

/* Static state for caching primes and found sequence elements */
static idx_t *prime_list = NULL; /* list of primes up to the current limit */
static size_t prime_cnt = 0;
static uint64_t sieve_limit = 2;        /* current upper bound for primes */
static uint64_t processed_limit = 0;    /* largest prime already examined */

static idx_t *found = NULL; /* primes that satisfy the condition */
static size_t found_len = 0;
static size_t found_cap = 0;

/*
 * Return the n-th term (1‑based) of the sequence.
 * Any order of calls is supported.  The function expands its sieve
 * until at least n terms are known.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1; harmless */

    while (found_len < n) {
        /* Build primes up to the current sieve limit */
        free(prime_list);               /* discard previous list */
        prime_list = NULL;
        prime_cnt = 0;
        build_primes(sieve_limit, (idx_t **)&prime_list, &prime_cnt);

        /* Examine only primes that have not yet been processed */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p <= processed_limit)
                continue;               /* already tested */

            processed_limit = p;        /* mark as processed */

            if ((p % 5) == 1) {         /* only these primes can fail */
                uint64_t exp = (p - 1) / 5;
                seq_t val = mod_pow((seq_t)21, exp, (seq_t)p);
                if (val != (seq_t)1) { /* 21 is not a 5th power residue */
                    /* Store the prime in the result array */
                    if (found_len == found_cap) {
                        found_cap = found_cap ? found_cap * 2 : 4;
                        found = (idx_t *)realloc(found,
                                found_cap * sizeof(idx_t));
                    }
                    found[found_len++] = (idx_t)p;
                    if (found_len >= n)
                        break;
                }
            }
        }

        if (found_len >= n)
            break; /* we have enough terms */

        /* Increase sieve limit and try again */
        sieve_limit *= 2;
    }

    return (seq_t)found[n - 1]; /* 1‑based indexing */
}
