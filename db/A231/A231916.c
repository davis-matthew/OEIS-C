/*
 * A231916  Primes p such that 16*p^2 + 10*p + 1 divides 2^p - 1.
 * https://oeis.org/A231916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 16*p^2 + 10*p + 1 divides 2^p -
 *            1
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *terms = NULL;     /* stored primes */
    static idx_t  count = 0;        /* number of primes stored */
    static size_t cap   = 0;        /* current allocation size */
    static uint64_t curr = 1;       /* last prime tested (initially 1) */

    if (n == 0) return 0;           /* guard for indices below offset */

    /* generate primes until we have at least n of them */
    while (count < n) {
        curr = next_prime(curr);    /* next prime > curr */

        /* compute m = 16*p^2 + 10*p + 1 using seq_t operations */
        seq_t p = (seq_t)curr;
        seq_t m = p * p;                 /* p^2 */
        m = m * (seq_t)16;               /* 16*p^2 */
        seq_t tenp = (seq_t)10 * p;      /* 10*p */
        m = m + tenp;                    /* 16*p^2 + 10*p */
        m = m + (seq_t)1;                /* +1 */

        /* check 2^p ≡ 1 (mod m) */
        seq_t r = mod_pow((seq_t)2, curr, m);
        if (r == (seq_t)1) {
            /* grow cache if necessary */
            if (count == cap) {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* allocation failed – clean up and return 0 */
                    free(terms);
                    terms = NULL;
                    count = 0;
                    cap = 0;
                    return 0;
                }
                terms = tmp;
                cap = newcap;
            }
            terms[count++] = p;
        }
    }

    return terms[n - 1];
}
