/*
 * A050691  Composite numbers whose decimal expansion starts with their
 *          largest prime divisor.
 * https://oeis.org/A050691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth composite integer whose decimal expansion starts
 *            with its largest prime divisor
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, largest_prime_divisor, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper function prototypes (implemented elsewhere) */
#include <stdint.h>
#include "../utils/largest_prime_divisor.h" /* uint64_t largest_prime_divisor(uint64_t m) */
#include "../utils/digits.h"                /* int digits(idx_t x) */
#include "../utils/pow10.h"                 /* seq_t pow10(idx_t e) */

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1 only. */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    uint64_t m = 4;  /* first composite integer */

    while (1) {
        /* Compute the largest prime divisor. */
        uint64_t p = largest_prime_divisor(m);

        /* Skip primes. */
        if (p != m) {
            /* Lengths of decimal representations. */
            int len_m = digits((idx_t)m);          /* number of digits of m  */
            int len_p = digits((idx_t)p);          /* number of digits of p  */

            /* Compute 10^(len_m - len_p) as a uint64_t. */
            int diff = len_m - len_p;
            if (diff > 0) {                       /* Always true for composites. */
                uint64_t pow_val = 1;
                for (int i = 0; i < diff; ++i) pow_val *= 10;

                /* Leading part of m after removing trailing digits. */
                uint64_t leading = m / pow_val;

                /* Check if the leading digits equal the largest prime divisor. */
                if (leading == p) {
                    ++count;
                    if (count == n) return (seq_t)m;
                }
            }
        }
        ++m;   /* Move to the next integer. */
    }
}
