/*
 * A223491  Largest Fermi-Dirac factor of n.
 * https://oeis.org/A223491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor of n of the form p^(2^k) (p prime, k ≥
 *            0). This is the same definition that defines OEIS sequence
 *            A223491
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/factor_u64.h"     /* factor_u64 */
#include "../utils/pow_uint.h"       /* pow_uint */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 1) {
        return (seq_t)1;
    }

    /* factor_u64 will write at most 16 distinct primes for any 64‑bit n. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t best = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* largest exponent of the form 2^k that does not exceed exps[i] */
        idx_t t = 1;
        while (t * 2 <= exps[i]) {
            t <<= 1;
        }

        /* compute p^t */
        seq_t val = pow_uint(primes[i], t);

        if (val > best) {
            best = val;
        }
    }

    return best;
}
