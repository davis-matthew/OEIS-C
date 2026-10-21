/*
 * A053164  4th root of largest 4th power dividing n.
 * https://oeis.org/A053164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^e∥n} p^{⌊e/4⌋}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_u64, _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_factor_u64.h"
#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    _factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e4 = exps[i] / 4;
        if (e4 == 0) {
            continue;
        }
        seq_t term = _pow_seq_t((seq_t)primes[i], e4);
        res *= term;
    }
    return res;
}
