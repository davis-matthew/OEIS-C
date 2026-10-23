/*
 * A176311  a(n) = sigma(n*2^n).
 * https://oeis.org/A176311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n * 2^n)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* compute exponent of prime 2 in n */
    idx_t e2 = 0;
    idx_t t = n;
    while (t > 0 && (t & 1) == 0) {
        e2++;
        t >>= 1;
    }

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t product = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];
        idx_t k = e;

        if (p == (seq_t)2) k += n;          /* exponent of 2 in n*2^n */

        seq_t term;
        if (p == (seq_t)2) {
            term = seq_pow((seq_t)2, k + 1) - (seq_t)1;
        } else {
            term = (seq_pow(p, k + 1) - (seq_t)1) / (p - 1);
        }
        product *= term;
    }

    /* if 2 does not divide n, add the 2^n factor */
    if (e2 == 0) {
        seq_t term = seq_pow((seq_t)2, n + 1) - (seq_t)1;
        product *= term;
    }

    return product;
}
