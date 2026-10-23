/*
 * A101472  Numbers k such that the number k33 is prime.
 * https://oeis.org/A101472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that concatenating k with "33" produces a prime.
   This is detected by testing primality of k * 100 + 33 for integer k. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset; safe fallback */

    uint64_t k = 1;
    idx_t count = 0;

    while (1) {
        uint64_t val = k * 100ULL + 33ULL;
        if (is_prime_uint64(val)) {
            if (++count == n) {
                return (seq_t)k;   /* found the n-th qualifying k */
            }
        }
        ++k;
    }
}
