/*
 * A201849  Primes of the form 7n^2 - 3.
 * https://oeis.org/A201849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime of the form 7*k^2 – 3 (OEIS A201849)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n = 0, safe fallback */

    idx_t count = 0;
    idx_t k = 1;

    while (1) {
        seq_t p = (seq_t)7 * ((seq_t)k * (seq_t)k) - 3;

        /* p fits in 64 bits for n up to at least 10 000 (checked by growth estimate) */
        if (is_prime64((uint64_t)p)) {
            ++count;
            if (count == n)
                return p;
        }
        ++k;
    }
}
