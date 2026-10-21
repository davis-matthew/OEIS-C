/*
 * A215041  a(n) = n^degree(C(n,x))/discriminant(C(n,x)) for the minimal
 *          polynomials C(n,x) of 2*cos(Pi/n), given in A187360.
 * https://oeis.org/A215041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^delta(n)/Discriminant(C(n,x)), where delta(n) = degree
 *            of the minimal polynomial of 2*cos(pi/n), i.e. delta(n) =
 *            phi(2n)/2 for n>1 and delta(1)=1. The sequence can be computed
 *            as follows: • if n==1: a(1)=1 • if n is a power of two, a(n)=2 •
 *            if n is a pure odd prime power p^k, a(n) = p^((...
 * Range:     u8 n <= 29, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* detect power of two */
    if ((n & (n - 1)) == 0)        /* n is 2^k */
        return (seq_t)2;

    /* factor n */
    const int MAX_FACTORS = 20;
    uint64_t primes[MAX_FACTORS];
    int      exps  [MAX_FACTORS];
    int       fcnt = 0;

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (fcnt < MAX_FACTORS) {
                primes[fcnt] = p;
                exps  [fcnt] = e;
                ++fcnt;
            }
        }
    }
    if (m > 1 && fcnt < MAX_FACTORS) {
        primes[fcnt] = m;
        exps  [fcnt] = 1;
        ++fcnt;
    }

    /* special case: pure odd prime power */
    if (fcnt == 1 && primes[0] != 2) {
        uint64_t p = primes[0];
        int      k = exps[0];
        /* exponent = (p^(k-1)+1)/2 */
        seq_t p_pow = big_pow((seq_t)p, (seq_t)(k - 1));
        seq_t exp   = (p_pow + 1) / 2;
        return big_pow((seq_t)p, exp);
    }

    /* compute phi(2n) */
    seq_t phi = (seq_t)1;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p    = primes[i];
        int      e    = exps[i];
        int      exp  = e;
        if (p == 2) exp++;                   /* exponent in 2n */

        seq_t pow_part = big_pow((seq_t)p, (seq_t)(exp - 1));
        seq_t contrib  = seq_mul(pow_part, (seq_t)(p - 1));
        phi = seq_mul(phi, contrib);
    }

    seq_t delta = phi / (seq_t)2;            /* delta(n) */

    /* general composite case: product over odd primes */
    seq_t result = (seq_t)1;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;               /* skip factor 2 */
        seq_t exp_term = delta / (seq_t)(p - 1);
        seq_t term = big_pow((seq_t)p, exp_term);
        result = seq_mul(result, term);
    }

    return result;
}
