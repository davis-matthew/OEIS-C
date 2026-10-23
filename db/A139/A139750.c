/*
 * A139750  Numbers n such that d_1^1 + ... + d_r^r is a prime number, where
 *          d_i are the digits of n (n = d_1, d_2, ..., d_r).
 * https://oeis.org/A139750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = the nth integer m such that Σ_{i=1}^{r} d_i^i is prime,
 *            where d_i are the digits of m (most significant first)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, get_digits, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_digits.h"
#include "../utils/big_pow.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache     = NULL;   /* array of found numbers     */
    static size_t cache_cap = 0;      /* allocated capacity         */
    static size_t cache_len = 0;      /* number of stored terms     */
    static idx_t  last_used = 0;      /* largest integer already tested */

    /* enlarge buffer when necessary */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find successive candidates until we have enough terms */
        while (cache_len < (size_t)n) {
            ++last_used;                  /* next integer */
            if (last_used == 0) continue; /* overflow to 0, skip */

            /* Extract digits (least‑significant first) */
            uint8_t digits[128];
            size_t len;
            get_digits((seq_t)last_used, digits, &len);

            /* Compute Σ d_i^i where i counts from 1 at the most significant digit */
            seq_t sum = 0;
            for (size_t idx = 0; idx < len; ++idx) {
                seq_t pow = big_pow((seq_t)digits[idx],
                                    (seq_t)(len - idx));   /* exponent = (#digits - idx) */
                sum += pow;
            }

            /* Test primality of the sum */
            if (isPrimeGeneric(sum)) {
                cache[cache_len++] = (seq_t)last_used;
            }
        }
    }

    return cache[(size_t)n - 1];
}
