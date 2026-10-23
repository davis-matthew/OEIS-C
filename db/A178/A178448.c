/*
 * A178448  Dirichlet inverse of A001160, sigma_5.
 * https://oeis.org/A178448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*mu(d)*d^5 Equivalent multiplicative
 *            description: a(1)=1, a(p) = -(1+p^5), a(p^2) = p^5, a(p^e)=0 for
 *            e>=3 (a(n) = product over prime powers in n of a(p^e))
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 73, i64 n <= 6169,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* Handle the offset; a(1) = 1 */
    if (n == 1) return 1;
    /* n must be >= 1; for n==0 we return 0 (not used) */
    if (n == 0) return 0;

    /* Factorise n: primes[i]^exps[i] for i=0..cnt-1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Use a 128‑bit accumulator when seq_t is 64‑bit or smaller
       to avoid intermediate overflow. */
    if (sizeof(seq_t) <= 8) {
        __int128 acc = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            if (e >= 3) return 0;          /* multiplicative zero */
            __int128 p5 = (__int128)p * p * p * p * p;   /* p^5 */
            __int128 factor = (e == 1) ? -(1 + p5) : p5; /* e==1 or 2 */
            acc *= factor;
        }
        return (seq_t)acc;
    }

    /* For larger seq_t (32, 64, 128, big‑int) use native type */
    seq_t acc = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e >= 3) return 0;
        seq_t p5 = pow_int((seq_t)p, 5);            /* p^5 */
        seq_t factor = (e == 1) ? -(1 + p5) : p5;
        acc *= factor;
    }
    return acc;
}
