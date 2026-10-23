/*
 * A193679  Sequence related to discriminant of cyclotomic polynomials
 *          A004124.
 * https://oeis.org/A193679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p|n} p^(phi(n)/(p-1))
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 44, u64 n <= 98, u128 n <= 164,
 *            big any n
 * Helpers:   calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper utilities */
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"

/* Fast exponentiation for seq_t */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* φ(n) */
    seq_t phi_n = calc_phi((seq_t)n);

    /* Factor n into prime factors */
    uint64_t primes[64];
    uint64_t exps[64];   /* unused, but required by factor_u64 signature */
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        seq_t exp_seq = phi_n / ((seq_t)(p - 1));
        idx_t exp = (idx_t)exp_seq;
        seq_t p_pow = pow_uint((seq_t)p, exp);
        result = result * p_pow;
    }
    return result;
}
