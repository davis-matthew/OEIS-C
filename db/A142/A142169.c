/*
 * A142169  Primes congruent to 17 mod 39.
 * https://oeis.org/A142169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 242, u32 n <= 243, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 17 mod 39 (OEIS A142169) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;   /* array of found primes */
    static size_t primes_cap = 0;  /* current capacity of array   */
    static size_t primes_cnt = 0;  /* number of primes stored     */
    static seq_t cand = 17;        /* next candidate   */

    /* Allocate initial storage if necessary */
    if (primes == NULL) {
        primes_cap = 64;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
    }

    /* Find primes until we have at least n of them */
    while (primes_cnt < (size_t)n) {
        if (is_prime_big(cand)) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* allocation failed – abort */
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = cand;
        }
        cand += (seq_t)39;  /* skip to next number ≡ 17 (mod 39) */
    }

    return primes[n - 1];
}
