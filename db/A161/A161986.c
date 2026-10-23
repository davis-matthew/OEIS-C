/*
 * A161986  a(n) = k+r where k is composite(n) and r is (largest prime divisor
 *          of k) mod (smallest prime divisor of k).
 * https://oeis.org/A161986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite(n) + (largest_prime_divisor(composite(n)) mod
 *            smallest_prime_divisor(composite(n)))
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _largest_prime_divisor_impl, _smallest_prime_factor_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_largest_prime_divisor_impl.h"
#include "../utils/_smallest_prime_factor_impl.h"

/* Return the nth value of OEIS A161986.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 4;                  /* first composite number */
    while (1) {
        uint64_t spf = _smallest_prime_factor_impl((uint64_t)i);
        if (spf != (uint64_t)i) {          /* i is composite */
            ++count;
            if (count == n) {
                uint64_t composite = (uint64_t)i;
                uint64_t lpf = _largest_prime_divisor_impl(composite);
                uint64_t r = lpf % spf;
                return (seq_t)(composite + r);
            }
        }
        ++i;
    }
}
