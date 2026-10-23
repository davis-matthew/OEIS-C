/*
 * A201708  Primes of the form 9n^2 + 10.
 * https://oeis.org/A201708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 9m²+10 (OEIS A201708)
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1205, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 1;
    while (count < n) {
        /* compute 9*m*m+10 as a 64‑bit integer, safe for n≤10⁴ */
        uint64_t p = 9ull * m * m + 10ull;
        if (is_prime(p)) {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
        ++m;
    }
    /* unreachable */
    return (seq_t)0;
}
