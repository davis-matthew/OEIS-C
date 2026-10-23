/*
 * A141952  Primes congruent to 7 mod 27.
 * https://oeis.org/A141952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 7 modulo 27 (OEIS A141952) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */

    /* memoised list of found primes */
    static seq_t *primes = NULL;
    static size_t cap   = 0;                /* allocated capacity */
    static size_t len   = 0;                /* number of primes stored */
    static idx_t  k_next = 0;               /* next k to test for candidate 27*k+7 */

    /* generate primes until we have at least n of them */
    while (len < n) {
        idx_t candidate = 27 * k_next + 7;
        if (is_prime(candidate)) {
            /* grow the array if needed */
            if (len == cap) {
                size_t newcap = cap ? cap << 1 : 32;
                seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return zero */
                    return (seq_t)0;
                }
                primes = tmp;
                cap    = newcap;
            }
            primes[len++] = (seq_t)candidate;
        }
        k_next++;
    }

    return primes[n - 1];
}
