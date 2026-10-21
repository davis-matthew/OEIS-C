/*
 * A079540  a(n) = phi(n) + d(n)*(n - phi(n)) - sigma(n).
 * https://oeis.org/A079540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) + d(n)*(n - phi(n)) - sigma(n)
 * Range:     u8 n <= 16, u16 n <= 2517, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   factor_u64, phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   phi   – Euler's totient function
 *   d     – number of divisors
 *   sigma – sum of divisors
 */

#include <stddef.h>
#include "../utils/phi.h"          /* seq_t phi(idx_t m); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/pow_uint.h"     /* seq_t pow_uint(uint64_t base, idx_t exp); */

/* Compute a(n) for n >= 1.  The function works for any supported
 * seq_t type and may be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value for n < 1 is irrelevant */

    /* Compute phi(n) using the provided routine. */
    seq_t phi_n = phi(n);
    seq_t n_seq = (seq_t)n;

    /* Factor n once and use the factorization to obtain d(n) and sigma(n). */
    static uint64_t primes_buf[128];
    static uint64_t exps_buf[128];
    size_t cnt;
    factor_u64((uint64_t)n, primes_buf, exps_buf, &cnt);

    /* Number of divisors d(n) = prod (exp_i + 1). */
    unsigned int d = 1;
    /* Sum of divisors sigma(n) = prod ((p_i^(exp_i+1)-1)/(p_i-1)). */
    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        unsigned int exp = (unsigned int)exps_buf[i];
        d *= (exp + 1);

        seq_t p_pow = pow_uint(primes_buf[i], exp + 1);
        seq_t term   = (p_pow - (seq_t)1) / (((seq_t)primes_buf[i]) - (seq_t)1);
        sigma *= term;
    }

    /* a(n) = phi(n) + d(n)*(n - phi(n)) - sigma(n) */
    seq_t result = phi_n + (seq_t)d * (n_seq - phi_n) - sigma;
    return result;
}
