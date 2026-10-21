/*
 * A230809  Primes p of the form 60*n + 59 such that 2*p + 1 is also prime.
 * https://oeis.org/A230809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p with p ≡ 59 (mod 60) such that 2p+1 is also
 *            prime
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap    = 0;        /* current capacity of cache */
    static idx_t   found  = 0;        /* number of terms already found */
    static idx_t   next_k = 2;        /* candidate index k  →  p = 60*k+59   */

    while (found < n) {
        /* build candidate prime p and its companion q = 2p+1 */
        seq_t p = (seq_t)60 * next_k + 59;
        seq_t q = p * ((seq_t)2) + 1;

        /* test primality of both numbers using the provided test */
        if (is_prime_big(p) && is_prime_big(q)) {
            if (found >= cap) {
                size_t new_cap = cap ? cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cap = new_cap;
            }
            cache[found++] = p;          /* store the term */
        }
        next_k++;                         /* advance to next candidate */
    }

    return cache[n - 1];
}
