/*
 * A096018  Number of Pythagorean quadruples mod n; i.e., number of solutions
 *          to w^2 + x^2 + y^2 = z^2 mod n.
 * https://oeis.org/A096018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   multiplicative a(2^e) = 2^(3e) for odd p≡1 (mod 4): a(p^e) =
 *            p^(2e-1) · (p^(e+1)+p^e-1) for odd p≡3 (mod 4): if e is even
 *            a(p^e) = p^(3e) - (p-1)p^(2e-1)(p^e-1)/(p+1) else (e odd) a(p^e)
 *            = p^(3e) - (p-1)p^(2e-1)(p^e+1)/(p+1)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1524, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/factor_u64.h"

static seq_t component(uint64_t p, int e)
{
    if (p == 2) {
        /* a(2^e) = 2^(3e) */
        return pow_seq((seq_t)2, 3ULL * (uint64_t)e);
    }

    if ((p & 3ULL) == 1ULL) { /* p ≡ 1 (mod 4) */
        seq_t p_pow_2e1 = pow_seq((seq_t)p, 2ULL * (uint64_t)e - 1ULL);
        seq_t p_pow_e   = pow_seq((seq_t)p, (uint64_t)e);
        seq_t p_pow_e1  = p_pow_e * (seq_t)p;           /* p^(e+1) */
        seq_t inner     = p_pow_e1 + p_pow_e - (seq_t)1;/* p^(e+1)+p^e−1 */
        return p_pow_2e1 * inner;
    }

    /* p ≡ 3 (mod 4) */
    seq_t p_pow_3e   = pow_seq((seq_t)p, 3ULL * (uint64_t)e);
    seq_t p_pow_2e1  = pow_seq((seq_t)p, 2ULL * (uint64_t)e - 1ULL);
    seq_t p_pow_e    = pow_seq((seq_t)p, (uint64_t)e);
    seq_t mult       = p_pow_2e1 * (seq_t)(p - 1ULL);  /* p^(2e-1)*(p-1) */

    if ((e & 1) == 0) {          /* even exponent: use (p^e-1)   */
        mult *= (p_pow_e - (seq_t)1);
    } else {                      /* odd exponent: use (p^e+1)    */
        mult *= (p_pow_e + (seq_t)1);
    }

    seq_t term = mult / (seq_t)(p + 1ULL);  /* integer division is exact */
    return p_pow_3e - term;
}

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= component(primes[i], (int)exps[i]);
    }
    return result;
}
