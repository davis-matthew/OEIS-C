/*
 * A090880  Suppose n=(p1^e1)*(p2^e2)*... where p1,p2,... are the prime
 *          numbers and e1,e2,... are nonnegative integers. Then a(n) = e1 +
 *          (e2)*3 + (e3)*9 + (e4)*27 + ... + (ek)*(3^(k-1)) + ...
 * https://oeis.org/A090880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = e1 + e2*3 + e3*9 + e4*27 + ... (exponents of n's prime
 *            factorisation, weighted by successive powers of 3)
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 78, u64 n <= 180,
 *            u128 n <= 420, big any n
 * Helpers:   big_pow, compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/compute_pi.h"   /* compute_pi(idx_t n) */
#include "../utils/big_pow.h"      /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0, values for n<1 are irrelevant */
        return (seq_t)0;

    /* Factorise n into primes and exponents */
    uint64_t primes[65];
    uint64_t exps[65];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* pi(p) = number of primes <= p  (index of p in the prime list) */
        seq_t pi = compute_pi(primes[i]);      /* seq_t, computed for arbitrary precision builds */
        seq_t exp = pi;
        exp -= (seq_t)1;                       /* exponent for the 3‑power is pi(p)‑1 */
        seq_t weight = big_pow((seq_t)3, exp); /* 3^(pi(p)-1) */

        sum += (seq_t)exps[i] * weight;        /* add e_i * 3^(i-1) */
    }
    return sum;
}
