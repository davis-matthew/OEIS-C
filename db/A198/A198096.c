/*
 * A198096  n such that d(1)*1! + d(2)*2! + ... + d(k)*k! is prime, where d(i)
 *          are the decimal digits of n.
 * https://oeis.org/A198096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198096
   a(n) = the n-th integer m such that
   sum_{i=1}^{k} d(i)*i!   is prime,
   where d(i) are the decimal digits of m (MSD first) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Next candidate to test */
    static seq_t next_candidate = 2;

    /* Ensure the cache contains at least n terms */
    while (cache_size < n) {
        seq_t m = next_candidate++;         /* candidate number */

        /* ------------------------------------------------------------------
           Compute the digit array of m (least‑significant digit first)
           ------------------------------------------------------------------ */
        seq_t temp = m;
        static uint8_t digits[4096];          /* big enough for typical terms */
        size_t len = 0;
        if (temp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (temp > 0) {
                digits[len++] = (uint8_t)(temp % 10);
                temp /= 10;
            }
        }

        /* ------------------------------------------------------------------
           Compute sum = d(1)*1! + d(2)*2! + ... + d(k)*k!
           Here d(1) is the most significant digit, so we traverse digits
           from the end of the array.
           ------------------------------------------------------------------ */
        seq_t fact = 1; /* factorial accumulator */
        seq_t sum  = 0;
        for (size_t i = 1; i <= len; ++i) {
            fact *= (seq_t)i;                            /* i!  */
            sum += (seq_t)digits[len - i] * fact;      /* d(i)*i! */
        }

        /* ------------------------------------------------------------------
           Test primality of the sum
           ------------------------------------------------------------------ */
        if (is_prime_big(sum)) {
            /* Grow cache if needed */
            if (cache_size >= cache_cap) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
            }
            cache[cache_size++] = m;
        }
    }

    return cache[n - 1];
}
