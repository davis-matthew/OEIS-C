/*
 * A118743  Primes of the form k!! + k + 1.
 * https://oeis.org/A118743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118743: primes of the form k!! + k + 1 (k = 0,1,2,…). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *realloc_seq(seq_t *p, size_t new_cap)
{
    void *tmp = realloc(p, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* memory allocation failed */
    return (seq_t *)tmp;         /* compatible with both C and C++ */
}

seq_t a(idx_t n)
{
    /* Static state: cache of found primes, current double‑factorial parts,
       and the next k to evaluate. */
    static seq_t *primes   = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static seq_t evenProd = 1;   /* product of even integers up to current k */
    static seq_t oddProd  = 1;   /* product of odd integers up to current k */
    static idx_t  curK     = 0;  /* smallest k not yet processed */

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        seq_t candidate;

        if (curK == 0) {                     /* 0!! = 1 */
            candidate = (seq_t)2;             /* 0!! + 0 + 1 = 2 */
        } else if (curK & 1) {                /* odd k */
            oddProd  = oddProd * (seq_t)curK;
            candidate = oddProd + (seq_t)curK + (seq_t)1;
        } else {                              /* even k */
            evenProd = evenProd * (seq_t)curK;
            candidate = evenProd + (seq_t)curK + (seq_t)1;
        }

        /* Test primality (works for all supported seq_t sizes). */
        if (is_prime_big(candidate)) {
            /* Grow the cache if needed. */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                primes = realloc_seq(primes, new_cap);
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
        curK++;
    }

    return primes[n - 1];
}
