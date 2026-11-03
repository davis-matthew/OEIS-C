/*
 * A017804  Binomial coefficients C(88,n).
 * https://oeis.org/A017804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(88,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 19, u128 n <= 88,
 *            big any n
 * Validated: full, pass, 89 terms through n = 88 (2026-11-02)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Exponent of prime p in n! (Legendre's formula) */
static idx_t _exponent_idx(idx_t n, idx_t p)
{
    idx_t exp = 0;
    idx_t power = p;
    while (power <= n) {
        exp += n / power;
        /* p <= 83 and n <= 88, multiplying power by p cannot overflow idx_t */
        power *= p;
    }
    return exp;
}

static const idx_t _primes_88[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67,
    71, 73, 79, 83
};

seq_t a(idx_t n)
{
    if (n > 88)
        return 0;

    /* Use symmetry to reduce the amount of work, but do not change the output */
    idx_t k = n;
    if (k > 44)               /* 88 - n < n  -> use smaller k  */
        k = 88 - n;

    /* Compute the binomial coefficient by prime factorisation */
    seq_t result = 1;

    size_t cnt = sizeof(_primes_88) / sizeof(_primes_88[0]);
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = _primes_88[i];

        /* exponent in C(88,k) = 88! / (k! (88-k)!) */
        idx_t exp = _exponent_idx(88, p) - _exponent_idx(k, p) - _exponent_idx(88 - k, p);
        if (exp == 0)
            continue;

        seq_t pow_p = ipow_idx((seq_t)p, exp);
        result = seq_mul(result, pow_p);
    }

    return result;
}
