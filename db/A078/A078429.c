/*
 * A078429  Number of integers k among 1..n for which gcd(k,n) is a cube.
 * https://oeis.org/A078429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{d|n, d is a perfect cube} φ(n/d)
 *            (Euler’s totient function).
 * This implementation uses the factorisation of n and
 * multiplies the contributions for each prime power.
 */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == (idx_t)0) /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Factorise n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t maxk = e / 3;          /* maximum k in d = p^(3k) */

        seq_t Si = (seq_t)0;
        for (uint64_t k = 0; k <= maxk; ++k) {
            uint64_t rem = e - 3 * k;   /* exponent of p in n/d */

            seq_t term;
            if (rem == 0) {
                /* φ(1) = 1  */
                term = (seq_t)1;
            } else {
                /* φ(p^rem) = (p-1) * p^(rem-1) */
                seq_t pseq = (seq_t)p;
                seq_t powv = pow_int(pseq, (idx_t)(rem - 1));
                term = ((seq_t)(p - 1)) * powv;
            }
            Si += term;
        }
        result *= Si;
    }

    return result;
}
