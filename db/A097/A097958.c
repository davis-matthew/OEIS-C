/*
 * A097958  Primes p such that p divides 6^((p-1)/2) - 3^((p-1)/2).
 * https://oeis.org/A097958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2414, u32 n <= 3052, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097958: primes p such that p | 6^((p-1)/2) – 3^((p-1)/2)
   (equivalently, p = 3 or p ≡ 1 or 7 (mod 8)) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;      /* cached list of terms */
static size_t   primes_cnt = 0;   /* number of cached terms   */
static size_t   primes_cap = 0;   /* allocated capacity       */
static seq_t    next_candidate = (seq_t)5;  /* next odd integer to test */

/* Ensure that at least n terms of the sequence are cached. */
static void ensure_primes(idx_t n)
{
    /* Initialise the cache if it is empty.  The first term is 3. */
    if (primes_cnt == 0) {
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) abort();          /* allocation failure */
        primes[0] = (seq_t)3;
        primes_cnt = 1;
        next_candidate = (seq_t)5;
    }

    while (primes_cnt < n) {
        /* Search the next qualifying prime. */
        seq_t p = next_candidate;
        for (;;) {
            if (is_prime_big(p)) {
                /* p ≡ 1 or 7 (mod 8) */
                if (((p & (seq_t)7) == (seq_t)1) || ((p & (seq_t)7) == (seq_t)7)) {
                    /* Grow the cache if necessary. */
                    if (primes_cnt == primes_cap) {
                        size_t new_cap = primes_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                        if (!tmp) abort();       /* allocation failure */
                        primes = tmp;
                        primes_cap = new_cap;
                    }
                    primes[primes_cnt++] = p;
                    next_candidate = p + (seq_t)2;   /* skip to the next odd number */
                    break;                          /* proceed to the next required term */
                }
            }
            p += (seq_t)2;  /* keep only odd numbers */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 for safety */
    ensure_primes(n);
    return primes[n - 1];
}
