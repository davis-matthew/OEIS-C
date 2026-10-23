/*
 * A029743  Primes with distinct digits.
 * https://oeis.org/A029743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = the n-th prime whose decimal digits are all distinct
 * Range:     u8 n <= 41, u16 n <= 2272, u32 n <= 9653, u64 n <= 9653,
 *            u128 n <= 9653, big any n
 * Helpers:   digit_frequencies, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 2;   /* next value to test */

    while (cache_len < n) {
        /* Check if last_checked is prime */
        if (is_prime(last_checked)) {
            /* Check if it has distinct digits */
            seq_t val = (seq_t)last_checked;
            unsigned int freq[10] = {0};
            digit_frequencies(val, freq);
            bool distinct = true;
            for (int d = 0; d < 10; ++d) {
                if (freq[d] > 1) {
                    distinct = false;
                    break;
                }
            }
            if (distinct) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = val;
            }
        }
        ++last_checked;
    }

    return cache[n - 1];
}
