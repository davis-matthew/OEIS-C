/*
 * A154390  Composites whose largest digit is prime.
 * https://oeis.org/A154390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:
   a(n) = the nth composite integer such that its largest decimal digit is a prime (2, 3, 5 or 7). */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    seq_t candidate = 12;         /* first composite with largest digit prime */
    idx_t count = 0;
    for (;;) {
        /* Composite test: composite iff not prime and greater than 1 (true for all candidates) */
        if (!is_prime_small(candidate)) {
            seq_t t = candidate;
            seq_t max_digit = 0;
            while (t > 0) {
                seq_t d = t % 10;
                if (d > max_digit) max_digit = d;
                t /= 10;
            }
            if (max_digit == 2 || max_digit == 3 ||
                max_digit == 5 || max_digit == 7) {
                ++count;
                if (count == n) return candidate;
            }
        }
        ++candidate;
    }
}
