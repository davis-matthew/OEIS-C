/*
 * A161594  a(n) = R(f(n)), where R = A004086 = reverse (decimal) digits, f =
 *          A071786 = reverse digits of prime factors.
 * https://oeis.org/A161594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = R(f(n))
 * Range:     u8 n <= 37, u16 n <= 1257, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 88 terms through n = 4543 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/reverse_digits.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Factor n into prime powers. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt;

    factor_u64(n, primes, exponents, &cnt);

    /* Compute f(n) by multiplying reversed primes raised to their exponents. */
    seq_t prod = 1;  /* seq_t started at 1 */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t revp = reverse_digits((seq_t)primes[i]);       /* R(p) */
        seq_t pw   = ipow_idx(revp, (idx_t)exponents[i]);     /* revp^exp */
        prod = seq_mul(prod, pw);
    }

    /* Return R(f(n)). */
    return reverse_digits(prod);
}
