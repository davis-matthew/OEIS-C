/*
 * A079635  Sum of (2 - p mod 4) for all prime factors p of n (with
 *          repetition).
 * https://oeis.org/A079635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{p^e‖n} e * (2 - (p mod 4))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (p=2 contributes 0; each prime factor counted with multiplicity) */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1)          /* offset: a(1) is first term, but handle 0 defensively */
        return (seq_t)0;

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        /* Compute 2 - (p mod 4) */
        uint64_t mod = p % 4;
        seq_t contrib;
        if (mod == 1)           /* prime ≡ 1 (mod 4) */
            contrib = 1;
        else if (mod == 3)      /* prime ≡ 3 (mod 4) */
            contrib = -1;
        else                    /* p == 2 or p ≡ 0 (mod 4) => contribution 0 */
            contrib = 0;
        sum += contrib * (seq_t)e;   /* add contribution with multiplicity */
    }
    return sum;
}
