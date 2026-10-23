/*
 * A155489  Primes p such that p and prime(p) have an odd largest digit.
 * https://oeis.org/A155489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, largest_digit_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A155489: primes p such that p and prime(p) have an odd largest digit. */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/largest_digit_seq.h"

seq_t a(idx_t n)
{
    /* Static state: cache of terms and primes. */
    static seq_t *terms = NULL;
    static size_t terms_sz = 0, terms_cap = 0;

    static uint64_t *primes = NULL;
    static size_t primes_sz = 0, primes_cap = 0;
    static uint64_t next_candidate = 2;
    static size_t eval_idx = 0;

    /* Ensure enough terms. */
    while (terms_sz < n)
    {
        /* Find next prime. */
        uint64_t cand = next_candidate;
        while (!is_prime(cand)) ++cand;
        next_candidate = cand + 1;

        /* Add cand to primes array. */
        if (primes_sz == primes_cap) {
            primes_cap = primes_cap ? primes_cap * 2 : 8;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_sz++] = cand;

        /* Evaluate pending primes that now have enough primes available. */
        while (eval_idx < primes_sz)
        {
            size_t p_val = primes[eval_idx];

            /* Wait until we have at least p_val primes to access prime(p). */
            if (primes_sz < p_val) break;

            bool odd_p = (largest_digit_seq((seq_t)p_val) & 1) != 0;
            if (odd_p) {
                seq_t prime_of_p = (seq_t)primes[p_val - 1];
                bool odd_q = (largest_digit_seq(prime_of_p) & 1) != 0;
                if (odd_q) {
                    if (terms_sz == terms_cap) {
                        terms_cap = terms_cap ? terms_cap * 2 : 8;
                        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    }
                    terms[terms_sz++] = (seq_t)p_val;
                }
            }
            ++eval_idx;
        }
    }

    return terms[n - 1];
}
