/*
 * A024901  Numbers k such that 7*k - 2 is prime.
 * https://oeis.org/A024901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 7*k - 2 is prime.
   The function scans successive integers k, testing 7*k-2 for primality
   until the nth such k is found. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = 1;
    idx_t count = 0;
    for (;;) {
        idx_t val = 7 * k - 2;
        if (is_prime(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
