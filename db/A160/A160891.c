/*
 * A160891  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 5.
 * https://oeis.org/A160891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_4(n)/phi(n) = ∏_{p^e‖n} p^(3e-3) * (1 + p + p^2 + p^3)
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 1049, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the elementary factorisation helper "factor_u64" and integer powers */
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n < 1)
        return 0;          /* defined only for n ≥ 1; return 0 otherwise */
    uint64_t num = (uint64_t)n;
    /* Factorise n; 64 is more than enough for distinct prime factors of a 64‑bit integer */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    seq_t result = 1;     /* multiplicative identity */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* exp = e - 1, then 3*(e-1) is exponent for the first term */
        idx_t e_minus1 = (idx_t)exps[i] - 1;
        idx_t exp_term = 3 * e_minus1;          /* 3(e-1) */
        seq_t term = pow_uint(p, exp_term);      /* p^(3(e-1)) */

        seq_t p1 = pow_uint(p, 1);   /* p^1    */
        seq_t p2 = pow_uint(p, 2);   /* p^2    */
        seq_t p3 = pow_uint(p, 3);   /* p^3    */

        seq_t sum = 1 + p1 + p2 + p3;    /* 1 + p + p^2 + p^3 */
        seq_t component = term * sum;    /* p^(3e-3) * (1+p+p^2+p^3) */

        result *= component;            /* multiply into final product */
    }
    return result;
}
