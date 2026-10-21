/*
 * A085232  In canonical prime factorization: power of smallest prime factor
 *          is less than power of greatest prime factor.
 * https://oeis.org/A085232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer whose smallest prime power is strictly
 *            less than its greatest prime power
 * Range:     u8 n <= 163, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches integers in increasing order,
   factorises them and tests the condition, memoising results. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;        /* memoised terms */
    static size_t term_len = 0;       /* number of stored terms */
    static size_t term_cap = 0;       /* capacity of terms array */

    /* Prime list for factorisation */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;     /* upper bound used for prime sieve */

    if (n == 0)
        return 0; /* undefined for offset 0, but guard against it */

    while (term_len < n) {
        /* Find the next candidate integer to test */
        uint64_t start = (term_len == 0) ? 2
                                          : (uint64_t)terms[term_len-1] + 1;
        for (uint64_t cand = start; ; ++cand) {
            /* Ensure we have enough primes for factoring cand */
            idx_t sqrt_lim = int_sqrt(cand) + 1;
            if (prime_limit < sqrt_lim) {
                if (prime_list)
                    free(prime_list);
                build_primes_upto(sqrt_lim, &prime_list, &prime_cnt);
                prime_limit = sqrt_lim;
            }

            /* Factor cand */
            size_t cnt = 0;
            uint64_t primes[64];
            uint64_t exps[64];
            factor_u64(cand, primes, exps, &cnt);

            /* Compute prime powers */
            seq_t pow_min = big_pow(primes[0], (seq_t)exps[0]);
            seq_t pow_max = big_pow(primes[cnt-1], (seq_t)exps[cnt-1]);

            if (pow_min < pow_max) {
                /* Candidate satisfies the condition */
                seq_t val = (seq_t)cand;
                if (term_len + 1 > term_cap) {
                    term_cap = term_cap ? term_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                }
                terms[term_len++] = val;
                break; /* move on to the next required term */
            }
        }
    }

    return terms[n-1];
}
