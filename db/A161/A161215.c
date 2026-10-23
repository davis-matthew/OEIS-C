/*
 * A161215  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 17.
 * https://oeis.org/A161215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 8, u128 n <= 159,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ((2^17-1)/phi(n)) * Sum_{d|n} μ(n/d) * d^16
 *
 * The sum simplifies to n^16 * ∏_{p|n} (1 - 1/p^16),
 * and φ(n) = n ∏_{p|n} (1 - 1/p).
 * Thus
 *   a(n) = 131071 * n^15 * ∏_{p|n} ( (p^16-1)/(p^15*(p-1)) )
 *        = 131071 * ∏_{p^e || n} p^{15(e-1)} * (p^16-1)/(p-1).
 *
 * The implementation uses this multiplicative form.
 */
#include <stdint.h>
#include "../utils/big_pow.h"   /* provides: seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    const seq_t C = (seq_t)131071;           /* 2^17-1 */
    uint64_t x = n;
    /* factor n into prime powers */
    uint64_t primes[32];
    uint64_t exps[32];
    int pcnt = 0;
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            primes[pcnt] = p;
            exps[pcnt]   = cnt;
            ++pcnt;
        }
    }
    if (x > 1) {
        primes[pcnt] = x;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t prod = (seq_t)1;
    for (int i = 0; i < pcnt; ++i) {
        seq_t p_t   = (seq_t)primes[i];
        uint64_t e  = exps[i];

        /* ratio = (p^16 - 1) / (p - 1)  = 1 + p + p^2 + ... + p^15   */
        seq_t p16   = big_pow(p_t, (seq_t)16);
        seq_t ratio = (p16 - (seq_t)1) / (p_t - (seq_t)1);

        /* power = p^(15*(e-1)) */
        uint64_t exp_pow = 15 * (e - 1);
        seq_t power = (seq_t)1;
        if (exp_pow > 0)
            power = big_pow(p_t, (seq_t)exp_pow);

        prod *= ratio * power;
    }

    prod *= C;
    return prod;
}
