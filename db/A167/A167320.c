/*
 * A167320  Totally multiplicative sequence with a(p) = 10*(p-3) for prime p.
 * https://oeis.org/A167320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Π_{p^e | n} (10·(p–3))^e , with a(3k)=0 for k≥1
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n==1 is the first term */
    if (n == 1) return 1;

    /* any multiple of 3 gives a zero factor */
    if (n % 3 == 0) return 0;

    /* factor n into primes and exponents (n fits in uint64_t) */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* p is never 3 here because of the earlier test, but keep check */
        if (p == 3) return 0;

        seq_t base = (seq_t)10 * ((seq_t)p - 3);
        uint64_t e = exps[i];
        for (uint64_t j = 0; j < e; ++j) {
            result *= base;
        }
    }
    return result;
}
