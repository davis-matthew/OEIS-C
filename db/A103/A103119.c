/*
 * A103119  Numbers k such that k0999 is prime.
 * https://oeis.org/A103119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer k such that the number k0999 is prime.
 *            The function computes the value by brute‑force search, which is
 *            fast enough for n up to about 10 000
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 1;

    while (count < n) {
        /* Form the number k0999.  For k up to ~1e6 this fits in uint64_t. */
        uint64_t val = k * 10000ULL + 999ULL;
        if (is_prime(val)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }

    /* The function should never reach here for n ≥ 1. */
    return (seq_t)0;
}
