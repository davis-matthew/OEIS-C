/*
 * A167319  Totally multiplicative sequence with a(p) = 9*(p-3) for prime p.
 * https://oeis.org/A167319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 9^Ω(n) * ∏(p-3)^(e) for each prime factor p^e of n;
 *            a(n)=0 if 3|n
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = -9, a(3) = 0, ... (OEIS A167319) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Ensure we work with unsigned 64‑bit integers for n */
    if (n == 0) return 0;
    if (n % 3 == 0) return 0;            /* any factor 3 gives a zero */

    /* Factor n into primes and exponents.  The helper fills simultaneous arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t base = 9 * (seq_t)(p - 3);   /* 9*(p-3) as a seq_t value */

        seq_t pow = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            pow *= base;                   /* compute base^exps[i] */
        }
        result *= pow;                     /* multiply into the running product */
    }
    return result;
}
