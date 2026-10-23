/*
 * A186293  a(n) = (A007519(n)-1)/2.
 * https://oeis.org/A186293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(1 mod 8, n) - 1)/2
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Computes the n-th term of OEIS sequence A186293.
   The sequence is defined as a(n) = (p_n - 1)/2, where p_n is the n-th
   prime congruent to 1 modulo 8. */
seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t vals_cap = 0;
    static size_t vals_cnt = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    static uint64_t cand = 3;   /* next odd candidate to test */

    /* initialize the primes list with 2 */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* generate values until we have at least n of them */
    while (vals_cnt < (size_t)n) {
        bool is_prime = true;

        /* trial division using known primes up to sqrt(cand) */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)  /* avoid overflow of p*p */
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store the new prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = cand;

            /* if prime ≡ 1 (mod 8), compute the sequence value */
            if ((cand & 7ULL) == 1ULL) {
                seq_t val = ((seq_t)cand - 1) / 2;
                if (vals_cnt == vals_cap) {
                    vals_cap = vals_cap ? vals_cap * 2 : 16;
                    vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                }
                vals[vals_cnt++] = val;
            }
        }

        /* next odd candidate */
        cand += 2;
    }

    return vals[n - 1];
}
