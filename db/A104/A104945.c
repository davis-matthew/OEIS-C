/*
 * A104945  Primes from merging of 2 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A104945: take the Champernowne constant 0.123456789101112..., 
   form overlapping two‑digit numbers from its decimal digits, ignore those 
   beginning with zero, and keep the primes among them.  a(n) is the nth prime
   thus obtained. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* count of primes found */
    idx_t count = 0;

    /* previous digit (none yet) */
    int prev = -1;

    /* iterate integers to generate Champernowne digits */
    for (uint64_t num = 1; ; ++num) {
        /* convert current integer to digits in reverse order */
        uint64_t t = num;
        int digits[20];
        int len = 0;
        while (t > 0) {
            digits[len++] = t % 10;
            t /= 10;
        }
        /* process digits in correct order */
        for (int i = len - 1; i >= 0; --i) {
            int cur = digits[i];
            if (prev >= 0) {
                if (prev != 0) {              /* skip pairs with leading 0 */
                    int pair = prev * 10 + cur;
                    if (is_prime((uint64_t)pair)) {
                        ++count;
                        if (count == n) {
                            return (seq_t)pair;
                        }
                    }
                }
            }
            prev = cur;
        }
    }
}
