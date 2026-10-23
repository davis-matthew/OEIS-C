/*
 * A184863  Numbers k such that floor(n+nr-r/2) are prime, where
 *          r=(1+sqrt(5))/2.
 * https://oeis.org/A184863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that floor(k*phi^2 - phi/2) is
 *            prime, where phi = (1+sqrt(5))/2. This is equivalent to
 *            floor((6k-1)+(2k-1)*sqrt(5))/4 being prime
 * Range:     u8 none, u16 n <= 6, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 1;       /* next integer k to test (1‑based) */

    /* Grow cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        /* Use the exact formula
           f(k) = floor((6k-1)+(2k-1)*sqrt(5)) / 4. */
        seq_t k = (seq_t)next_k;

        seq_t a_val = ((seq_t)6 * k) - 1;        /* a = 6k-1  */
        seq_t b_val = ((seq_t)2 * k) - 1;        /* b = 2k-1  */

        /* temp = 5*b*b, whose square root floor is floor(b*sqrt(5)) */
        seq_t temp = ((seq_t)5 * b_val) * b_val;

        seq_t floor_b = floor_sqrt(temp);      /* floor(b*sqrt(5)) */

        seq_t sum = a_val + floor_b;           /* a + floor(b*sqrt(5)) */

        /* t is sum mod 4; we can use masking because 4 is a power of two. */
        seq_t t = sum & 3;                     /* 0 <= t <= 3 and (sum-t) divisible by 4 */

        seq_t m = (sum - t) >> 2;             /* floor((a+floor(b*sqrt(5)))/4) */

        /* If the value is prime, store the corresponding k. */
        if (is_prime_big(m)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)next_k);
        }

        ++next_k; /* test next integer */
    }

    /* n is 1‑based, but the array is 0‑based. */
    return cache[(size_t)n - 1];
}
