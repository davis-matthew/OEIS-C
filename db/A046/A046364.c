/*
 * A046364  Odd composite numbers whose sum of prime factors is prime (counted
 *          with multiplicity).
 * https://oeis.org/A046364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046364: odd composite numbers whose sum of prime factors (with multiplicity) is prime. 
   The function computes terms by scanning odd integers and selecting those with the required property. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 1; /* will be incremented to 3 on first use */

    size_t idx = (size_t)n;
    if (cache_len > idx) return cache[idx];

    while (cache_len <= idx) {
        /* Find next odd number */
        do {
            last_checked += 2;
        } while ((last_checked & 1ULL) == 0);

        /* Skip if it is prime */
        if (isPrimeTrial((seq_t)last_checked)) continue;

        /* Sum of prime factors (with multiplicity) */
        uint64_t temp = last_checked;
        uint64_t sum = 0;

        while ((temp & 1ULL) == 0) {    /* factor 2 */
            sum += 2;
            temp >>= 1;
        }

        uint64_t p;
        for (p = 3; p * p <= temp; p += 2) {
            while (temp % p == 0) {
                sum += p;
                temp /= p;
            }
        }
        if (temp > 1) sum += temp;

        /* Check if the sum is prime */
        if (isPrimeTrial((seq_t)sum)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[idx];
}
