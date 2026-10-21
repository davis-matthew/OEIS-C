/*
 * A084317  Concatenation of the prime factors of n, in increasing order.
 * https://oeis.org/A084317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 857, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = concatenation of the distinct prime factors of n in increasing order.
   a(1) = 0. For n > 1, prime factors are factorized, sorted increasingly,
   and their decimal representations are concatenated to form the result. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Count decimal digits of a positive 64‑bit integer. */
static inline int digits_uint64(uint64_t x) {
    int d = 1;
    while (x >= 10ULL) {
        x /= 10ULL;
        d++;
    }
    return d;
}

seq_t a(idx_t n) {
    /* 1 has no prime factors → 0 */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Factorisation: primes[] holds distinct prime factors, exps[] the
       corresponding exponents (unused).  Size limited to 32 distinct primes
       which is enough for every n that fits in uint64_t. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    /* Use the generic 64‑bit factoriser from the utils library. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int d = digits_uint64(p);

        seq_t pow10 = 1;
        for (int j = 0; j < d; ++j) {
            pow10 *= 10;           /* multiply by 10 d times */
        }

        if (result == 0) {
            result = (seq_t)p;     /* first prime, just copy */
        } else {
            result = result * pow10 + (seq_t)p;
        }
    }
    return result;
}
