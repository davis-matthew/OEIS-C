/*
 * A045407  Primes congruent to {0, 1, 3} mod 5.
 * https://oeis.org/A045407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p with p ≡ 0,1,3 (mod 5)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of already found terms   */
    static size_t cap   = 0;         /* current capacity of cache      */
    static idx_t  count = 0;         /* number of terms stored         */
    static idx_t  candidate = 2;     /* next integer to test           */

    if (n <= 0) return 0;           /* defined for n>=1, but safe     */

    /* Extend cache if needed */
    if (cache == NULL) {
        cap = n > 128 ? n : 128;                /* initial capacity          */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* out of memory             */
    }

    /* Iterate until we have computed the nth term */
    while (count < n) {
        /* Skip non‑primes or primes not ≡ 0,1,3 mod 5 */
        while (!is_prime(candidate) ||
               !(candidate % 5 == 0 || candidate % 5 == 1 || candidate % 5 == 3))
            ++candidate;

        /* Store the found prime */
        if (count >= cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            if (tmp == NULL) return 0;           /* out of memory */
            cache = tmp;
        }
        cache[count] = (seq_t)candidate;
        ++count;
        ++candidate;
    }

    return cache[n - 1];
}
