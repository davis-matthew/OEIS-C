/*
 * A068720  Arithmetic derivative of squares: a(n) = 2*n*A003415(n).
 * https://oeis.org/A068720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003415(n^2) = 2*n*A003415(n)
 * Range:     u8 n <= 11, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"


seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Arrays large enough for any 64‑bit n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t nseq = (seq_t)n;
    seq_t res  = 0;

    for (size_t i = 0; i < cnt; ++i) {
        idx_t np = n / primes[i];          /* n / p is integer              */
        seq_t term = 2 * nseq * (seq_t)exps[i] * (seq_t)np;
        res += term;
    }
    return res;
}
