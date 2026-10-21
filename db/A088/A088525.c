/*
 * A088525  Duplicate of A054735.
 * https://oeis.org/A088525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 * (p_n + 1), where p_n is the n-th first member of a
 *            twin‑prime pair (i.e. the n-th prime p such that p + 2 is also
 *            prime)
 * Range:     i8 n <= 7, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to test primality using a dynamically growing list of known primes */
static bool is_prime_idx(idx_t x, idx_t *primes, size_t primes_count)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        if (p > x / p) break;          /* p*p > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state: twin‑prime first elements */
    static idx_t  *twin_cache = NULL;
    static size_t twin_cap   = 0;
    static size_t twin_size  = 0;

    /* Static state for prime list */
    static idx_t *prime_list = NULL;
    static size_t primes_cap = 0;
    static size_t primes_size = 0;

    /* Where to start the search for more twin primes */
    static idx_t next_candidate = 3;   /* odd number >= 3 */

    /* Expand cache until we have at least n entries */
    while (twin_size < n) {
        /* Test whether next_candidate is prime */
        bool cand_is_prime;
        if (next_candidate == 3) {
            /* 3 is known to be prime */
            cand_is_prime = true;
        } else {
            cand_is_prime = is_prime_idx(next_candidate, prime_list, primes_size);
        }

        /* If candidate is prime, add to prime list */
        if (cand_is_prime) {
            if (primes_size >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                prime_list = (idx_t *)realloc(prime_list,
                                              primes_cap * sizeof(idx_t));
            }
            prime_list[primes_size++] = next_candidate;
        }

        /* If candidate is prime, check if candidate + 2 is also prime */
        if (cand_is_prime) {
            if (is_prime_idx(next_candidate + 2, prime_list, primes_size)) {
                /* Store this twin‑prime first member */
                if (twin_size >= twin_cap) {
                    twin_cap = twin_cap ? twin_cap * 2 : 16;
                    twin_cache = (idx_t *)realloc(twin_cache,
                                                  twin_cap * sizeof(idx_t));
                }
                twin_cache[twin_size++] = next_candidate;
            }
        }

        /* Advance to next odd candidate */
        next_candidate += 2;
    }

    /* Compute a(n) = 2 * (p_n + 1) */
    idx_t p = twin_cache[n - 1];
    seq_t res = (seq_t)2 * ((seq_t)p + (seq_t)1);
    return res;
}
