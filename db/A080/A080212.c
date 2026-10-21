/*
 * A080212  Binomial(n, smallest odd prime factor of n).
 * https://oeis.org/A080212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, p(n)) where p(n) is the smallest odd prime
 *            factor of n, and p(n)=1 when n has no odd prime factor (e.g.
 *            powers of 2)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom_combo
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_combo.h"

/* Find smallest odd prime factor of n; if none, return 1. */
static idx_t smallest_odd_prime_factor(idx_t n) {
    /* remove all factors of 2 */
    uint64_t rem = (uint64_t)n;
    while ((rem & 1ULL) == 0ULL) rem >>= 1;
    if (rem == 1ULL) return 1;          /* no odd prime factor */
    /* trial division by odd numbers */
    for (idx_t d = 3; (uint64_t)d * (uint64_t)d <= rem; d += 2) {
        if (rem % d == 0ULL) return d;
    }
    return (idx_t)rem;                  /* rem itself is odd prime */
}

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)n;        /* a(1)=1, a(2)=2 */
    idx_t p = smallest_odd_prime_factor(n);
    return binom_combo(n, p);
}
