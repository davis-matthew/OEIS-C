/*
 * A142769  Primes congruent to 42 mod 59.
 * https://oeis.org/A142769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p ≡ 42 (mod 59) */

/* The function returns the n‑th prime congruent to 42 modulo 59.
   It memoizes results to be efficient for repeated, out‑of‑order calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) is available */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 as safe default */

    /* static memoization state */
    static seq_t *primes = NULL;
    static idx_t  count   = 0;    /* number of primes stored so far */
    static idx_t  cap     = 0;    /* current allocated capacity */
    static idx_t  next_i  = 0;    /* next integer i for candidate 42 + 59*i */

    /* initialize storage on first call */
    if (primes == NULL) {
        cap = 16;
        primes = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!primes) return (seq_t)0; /* allocation failed */
    }

    /* generate more primes until we have at least n of them */
    while (count < n) {
        /* candidate = 42 + 59*i */
        seq_t cand = (seq_t)42 + ((seq_t)next_i * (seq_t)59);
        next_i++;

        if (is_prime_big(cand)) {
            /* store the prime */
            if (count >= cap) {
                /* grow capacity: double it */
                idx_t new_cap = cap * 2;
                seq_t *tmp = (seq_t*)realloc(primes, sizeof(seq_t) * new_cap);
                if (!tmp) return (seq_t)0; /* allocation failed */
                primes = tmp;
                cap = new_cap;
            }
            primes[count++] = cand;
        }
    }

    return primes[n - 1];
}
