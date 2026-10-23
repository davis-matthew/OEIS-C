/*
 * A141182  Primes of the form x^2+6*x*y-2*y^2 (as well as of the form
 *          5*x^2+8*x*y+y^2).
 * https://oeis.org/A141182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1547, u32 n <= 1587, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code finds primes p such that p = x^2+6*x*y-2*y^2 (equivalently u^2-11*v^2).
   Equivalent conditions: prime, p ≡ 1 (mod 4) and p ≡ 1,3,4,5,9 (mod 11). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t); */

seq_t a(idx_t n) {
    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t candidate = 5;

    /* generate primes until we have at least n of them */
    while (len < n) {
        if ((candidate % 4ULL) == 1) {
            seq_t m11 = candidate % 11ULL;
            if (m11 == 1 || m11 == 3 || m11 == 4 || m11 == 5 || m11 == 9) {
                if (is_prime_big(candidate)) {
                    /* store in cache */
                    if (len == cap) {
                        size_t newcap = cap ? cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
                        if (!tmp) abort();
                        primes = tmp;
                        cap = newcap;
                    }
                    primes[len++] = candidate;
                }
            }
        }
        /* advance to next odd number */
        candidate += 2ULL;
    }

    return primes[n - 1];
}
