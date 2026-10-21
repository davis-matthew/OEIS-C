/*
 * A053575  Odd part of phi(n): a(n) = A000265(A000010(n)).
 * https://oeis.org/A053575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = odd part of φ(n) = A000265(A000010(n))
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/factor_uint64.h"

seq_t a(idx_t n) {
    /* Special case n = 0 never occurs, but keep safe */
    if (n == 0) return (seq_t)0;

    /* Factor n.  factor_uint64 expects arrays of at least 20 entries. */
    uint64_t fac[20];
    int exp[20];
    int cnt;

    factor_uint64((uint64_t)n, fac, exp, &cnt);

    /* Compute φ(n) = n * ∏(p−1)/p over distinct primes p|n */
    seq_t phi = (seq_t)n;
    for (int i = 0; i < cnt; i++) {
        uint64_t p = fac[i];
        /* Division first to keep intermediate values small */
        phi = phi / (seq_t)p * (seq_t)(p - 1);
    }

    /* Strip all powers of 2 to obtain the odd part */
    while ((phi & (seq_t)1) == 0) {
        phi >>= 1;
    }

    return phi;
}
