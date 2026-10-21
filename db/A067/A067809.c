/*
 * A067809  Composite n such that sigma(3*n) < prime(n).
 * https://oeis.org/A067809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A067809 – Composite n such that σ(3·n) < pₙ  
    where σ is the sum–of–divisors function and pₙ is the n‑th prime.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n);            */
#include "../utils/sigma_of.h"     /* seq_t sigma_of(idx_t n);           */
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k);      */

/* Cache of terms already found, ordered by index. */
static seq_t *cached   = NULL;   /* array of a(k) values           */
static size_t cached_cap = 0;    /* allocated capacity of cached   */
static idx_t   cached_len = 0;   /* number of terms stored         */
static idx_t   last_tested = 1;  /* largest integer examined so far */

/* Return the n‑th term of A067809 (n >= 1). */
seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n terms. */
    while (cached_len < n) {
        /* Next integer to examine. */
        last_tested++;

        /* Skip primes – we only need composites. */
        if (is_prime(last_tested))
            continue;

        /* Compute σ(3·last_tested) and compare with the n‑th prime. */
        idx_t product = (idx_t)3 * last_tested;
        seq_t sigma   = sigma_of(product);
        seq_t prime   = (seq_t)get_prime(last_tested);

        if (sigma < prime) {
            /* Add the new term to the cache. */
            if (cached_len == cached_cap) {
                /* Grow capacity, starting from 16 or doubling. */
                cached_cap = cached_cap ? cached_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cached,
                                             cached_cap * sizeof(seq_t));
                if (!tmp) abort();
                cached = tmp;
            }
            cached[cached_len++] = last_tested;
        }
    }

    return cached[n - 1];
}
