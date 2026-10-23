/*
 * A121373  Expansion of f(x) = f(x, -x^2) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A121373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(24*n+1), where b is multiplicative with * b(p^(2e)) =
 *            (-1)^e if p ≡ 7,11,13,17 (mod 24), * b(p^(2e)) = +1 if p ≡
 *            1,5,19,23 (mod 24), * b(p^odd) = 0 for all odd prime powers. *
 *            Implementation factors 24*n+1 and applies the above rule
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /*  m = 24*n + 1  (uint64_t avoids overflow for small n) */
    uint64_t m = (uint64_t)n * 24ULL + 1ULL;
    if (m == 1ULL) return (seq_t)1;   /* a(0) */

    /* factorise m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;   /* result accumulates sign, or 0 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* any odd exponent => factor = 0 */
        if (e & 1ULL) return (seq_t)0;

        /* even exponent: determine sign contribution */
        uint64_t half = e >> 1;
        uint64_t mod  = p % 24ULL;

        if (mod == 7ULL || mod == 11ULL || mod == 13ULL || mod == 17ULL) {
            if (half & 1ULL) result = -result;   /* multiply by (-1)^half */
        } else if (mod == 1ULL || mod == 5ULL || mod == 19ULL || mod == 23ULL) {
            /* multiply by +1 ⇒ nothing to do */
        }
        /* 24*n+1 is coprime to 2 and 3, so no other residue classes occur */
    }

    return result;
}
