/*
 * A132882  a(n) = the sum of the positive isolated divisors of n.
 * https://oeis.org/A132882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of isolated positive divisors of n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isolated_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A divisor d of n is isolated if neither (d‑1) nor (d+1) divides n.
   By convention 1 is isolated iff n is odd. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isolated_divisor.h"

seq_t a(idx_t n) {
    if (n < 1u) {
        /* Undefined for n = 0 in OEIS definition; return 0. */
        return 0;
    }

    seq_t sum = 0;

    for (idx_t i = 1u; i * i <= n; ++i) {
        if (n % i == 0u) {
            idx_t d1 = i;
            if (isolated_divisor(n, d1)) {
                sum += (seq_t)d1;
            }

            idx_t d2 = n / i;
            if (d2 != d1 && isolated_divisor(n, d2)) {
                sum += (seq_t)d2;
            }
        }
    }

    return sum;
}
