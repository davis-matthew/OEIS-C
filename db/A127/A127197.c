/*
 * A127197  a(n) = numerator( pi(n)/2^n ).
 * https://oeis.org/A127197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( pi(n)/2^n )
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 88 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Compute the prime‑counting function π(n). */
    seq_t pi_val = prime_pi(n);

    /* Since π(n) is tiny (≤ 1229 for n ≤ 10000), we may cast to uint64_t. */
    uint64_t v = (uint64_t)pi_val;

    /* Divide out all factors of 2 to obtain the numerator in lowest terms. */
    while ((v & 1) == 0) {
        v >>= 1;
    }

    return (seq_t)v;
}
