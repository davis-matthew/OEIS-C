/*
 * A142029  Primes congruent to 25 mod 31.
 * https://oeis.org/A142029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   prime congruent to 25 mod 31. Search by trial division
 * Range:     u8 n <= 2, u16 n <= 197, u32 n <= 198, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored sequence values   */
    static size_t cache_len = 0;         /* number of values cached */
    static size_t cache_cap = 0;         /* capacity of cache       */

    /* generate values until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* start searching from just after the last stored prime   */
        seq_t cand;
        if (cache_len == 0)
            cand = (seq_t)25;             /* first number ≡25 (mod 31)   */
        else
            cand = cache[cache_len - 1] + (seq_t)2;  /* next odd following last prime */

        /* iterate over odd numbers, testing primality and the mod condition */
        for (;; cand = cand + (seq_t)2) {
            if ((cand % (seq_t)31) == (seq_t)25 && is_prime_big(cand)) {
                /* grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();                       /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;   /* store the found prime */
                break;                         /* proceed to next term  */
            }
        }
    }

    return cache[n - 1];
}
