/*
 * A130030  a(n) = Sum_{d|n} (-1)^omega(n/d) * phi(rad(n/d)) * prime(d).
 * https://oeis.org/A130030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} (-1)^ω(n/d) * φ(rad(n/d)) * prime(d)
 * Range:     i8 n <= 52, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(k) = k-th prime, ω(m) = #{distinct primes|m} */
/* Uses the provided utilities factor_u64 and get_prime. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

/* Recursive enumeration of all divisors of n, using its prime factorisation. */
static void dfs(const idx_t *pf, const idx_t *exp_tot, idx_t depth, idx_t k,
                idx_t curr_d, idx_t *exp_d, seq_t *sum)
{
    if (depth == k) {
        seq_t sign = 1;
        seq_t phi  = 1;
        for (idx_t i = 0; i < k; ++i) {
            if (exp_tot[i] > exp_d[i]) {
                sign = -sign;                     /* toggle sign for each distinct prime of n/d */
                phi  *= (seq_t)(pf[i] - 1);       /* φ(rad(n/d)) multiplies (p-1) for each distinct prime */
            }
        }
        seq_t prime_d = (seq_t)get_prime(curr_d);
        *sum += sign * phi * prime_d;
        return;
    }

    idx_t p   = pf[depth];
    idx_t pow = 1;
    for (idx_t e = 0; e <= exp_tot[depth]; ++e) {
        exp_d[depth] = e;
        dfs(pf, exp_tot, depth + 1, k, curr_d * pow, exp_d, sum);
        if (e < exp_tot[depth]) pow *= p;        /* prepare next power of p */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for 0, harmless */

    /* Factorise n */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t   cnt = 0;
    factor_u64(n, primes_buf, exps_buf, &cnt);

    /* Convert to idx_t arrays */
    idx_t pf[64];
    idx_t exp_tot[64];
    for (size_t i = 0; i < cnt; ++i) {
        pf[i]      = (idx_t)primes_buf[i];
        exp_tot[i] = (idx_t)exps_buf[i];
    }

    idx_t exp_d[64];   /* will hold the exponent vector for each divisor */
    seq_t sum = 0;
    dfs(pf, exp_tot, 0, (idx_t)cnt, 1, exp_d, &sum);
    return sum;
}
