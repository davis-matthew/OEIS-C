/*
 * A216664  Odd numbers n such that the decimal expansion of 1/n contains the
 *          digit "9" at position (n + 1)/2.
 * https://oeis.org/A216664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th odd integer such that the decimal expansion of
 *            1/n contains the digit "9" at position (n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 500, u32 n <= 692, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/powmod_mod.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t capacity   = 0;

    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* find terms up to n, if not already cached */
    static uint64_t candidate = 1;   /* next odd number to test (starts with 1) */

    while (cache_size < (size_t)n) {
        /* advance to next odd number */
        candidate += 2;
        /* skip 1, because the first term is 11 */
        if (candidate < 3)
            candidate = 3;

        idx_t k = (candidate + 1) / 2;               /* position to check */
        seq_t mod = (seq_t)10 * candidate;           /* 10*n modulus */

        seq_t pow = powmod_mod((seq_t)10, k, mod);
        seq_t digit = pow / candidate;              /* floor division gives the k-th digit */

        if (digit == 9) {
            if (cache_size == capacity) {           /* grow cache as needed */
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];
}
