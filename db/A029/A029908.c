/*
 * A029908  Starting with n, repeatedly sum prime factors (with multiplicity)
 *          until reaching 0 or a fixed point. Then a(n) is the fixed point
 *          (or 0).
 * https://oeis.org/A029908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = limit of repeated application of SOPFR
   (sum of prime factors with multiplicity) to n, stopping when
   SOPFR(x) = x or x = 0.  For n>1 the fixed point is either 4
   or a prime. */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

static inline seq_t sopfr_u64(uint64_t u) {
    seq_t sum = 0;
    while (u > 1) {
        uint64_t p = _smallest_prime_factor(u);
        sum += (seq_t)p;
        u /= p;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* below offset */
    }
    seq_t cur = (seq_t)n;
    for (;;) {
        if (cur == 0) {
            return 0;
        }
        seq_t next = sopfr_u64((uint64_t)cur);
        if (next == cur) {
            return next;
        }
        cur = next;
    }
}
