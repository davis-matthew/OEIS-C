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
 * Helpers:   digit_prod, is_prime_candidate
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
#include "../utils/digit_prod.h"
#include "../utils/is_prime_candidate.h"

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
        prime_list = (seq_t*)malloc(prime_cap * sizeof(seq_t));
    }
    /* expand term list if needed */
    if (term_list == NULL) {
        term_cap = 512;
        term_list = (seq_t*)malloc(term_cap * sizeof(seq_t));
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
            prime_list = (seq_t*)realloc(prime_list, prime_cap * sizeof(seq_t));
        }
        prime_list[prime_cnt++] = candidate;

        /* test condition */
        seq_t prod = digit_prod(candidate);
        if (prod != 0 && (candidate - 1) % prod == 0) {
            if (term_cnt == term_cap) {
                term_cap *= 2;
                term_list = (seq_t*)realloc(term_list, term_cap * sizeof(seq_t));
            }
            term_list[term_cnt++] = candidate;
        }

        ++candidate;
    }

    return term_list[(size_t)n - 1];
}
