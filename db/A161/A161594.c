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
 * Helpers:   factor_u64, reverse_decimal, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/reverse_decimal.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

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
        seq_t revp = reverse_decimal((seq_t)primes[i]);       /* R(p) */
        seq_t pw   = seq_pow(revp, (idx_t)exponents[i]);     /* revp^exp */
        prod = seq_mul(prod, pw);
    }

    /* Return R(f(n)). */
    return reverse_decimal(prod);
}
