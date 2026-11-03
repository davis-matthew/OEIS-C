/*
 * A081984  Primes p such that p-1 is divisible by the digital product ( of
 *          nonzero digits) of p.
 * https://oeis.org/A081984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A081984
 * Definition: a(n) = the nth prime p such that (p-1) is divisible by the product
 *              of the non‑zero decimal digits of p.
 * Computation: search for prime numbers in increasing order, test the condition
 *              and cache all found terms.  The function is thread‑local via
 *              static storage and can answer future calls in O(1).
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute product of non‑zero decimal digits of x */
static seq_t digit_prod(seq_t x)
{
    seq_t prod = 1;
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d != 0) prod *= d;
        x /= 10;
    }
    return prod;
}

/* Helper: test primality using found prime list */
static int is_prime_candidate(seq_t x, seq_t *primes, size_t primes_used)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < primes_used; ++i) {
        seq_t p = primes[i];
        if (p > x / p) break;          /* avoid overflow in p*p > x */
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of primes and valid terms */
    static seq_t *prime_list = NULL;
    static size_t prime_cap  = 0;
    static size_t prime_cnt  = 0;

    static seq_t *term_list  = NULL;
    static size_t term_cap   = 0;
    static size_t term_cnt   = 0;

    /* expand prime list if needed */
    if (prime_list == NULL) {
        prime_cap = 1024;
        prime_list = mem_new(seq_t, prime_cap);
    }
    /* expand term list if needed */
    if (term_list == NULL) {
        term_cap = 512;
        term_list = mem_new(seq_t, term_cap);
    }

    /* generate terms until we have at least n */
    seq_t candidate = 2;

    /* skip already examined primes */
    if (prime_cnt > 0) candidate = prime_list[prime_cnt - 1] + 1;
    else candidate = 2;

    while (term_cnt < (size_t)n) {
        /* find next prime */
        while (!is_prime_candidate(candidate, prime_list, prime_cnt)) {
            ++candidate;
        }

        /* store prime */
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_list = mem_resize(seq_t, prime_list, prime_cap);
        }
        prime_list[prime_cnt++] = candidate;

        /* test condition */
        seq_t prod = digit_prod(candidate);
        if (prod != 0 && (candidate - 1) % prod == 0) {
            if (term_cnt == term_cap) {
                term_cap *= 2;
                term_list = mem_resize(seq_t, term_list, term_cap);
            }
            term_list[term_cnt++] = candidate;
        }

        ++candidate;
    }

    return term_list[(size_t)n - 1];
}
