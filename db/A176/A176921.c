/*
 * A176921  Period of decimal representation of 1/n^3.
 * https://oeis.org/A176921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factor_u64, mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the period of the decimal representation of 1/n³.
   It removes all factors 2 and 5 from n³ (since these determine the
   terminating part), then finds the multiplicative order of 10 modulo
   the remaining part.  The order is obtained by factoring φ(m) and
   successively dividing by its prime factors.  All arithmetic is
   performed with the seq_t type. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* build m = n³ with 2 and 5 removed */
    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i] * 3;   /* exponent in n³ */
        if (p == 2 || p == 5) {
            continue;            /* drop all 2's and 5's */
        }
        seq_t pe = pow_uint(p, e);
        m *= pe;
    }
    if (m == 1) return 1;   /* terminating decimal */

    /* compute φ(m) */
    seq_t phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i] * 3;
        if (p == 2 || p == 5) {
            continue;
        }
        seq_t p_pow_minus1 = pow_uint(p, e - 1); /* p^(e-1) */
        seq_t term = p_pow_minus1 * (seq_t)(p - 1); /* (p-1)*p^(e-1) */
        phi *= term;
    }

    /* factor φ(m) (it fits in uint64_t because n² < 1e12 for n≤10000) */
    uint64_t pr_phi[64];
    uint64_t ex_phi[64];
    size_t cnt_phi = 0;
    factor_u64((uint64_t)phi, pr_phi, ex_phi, &cnt_phi);

    /* compute multiplicative order of 10 modulo m */
    seq_t order = phi;
    for (size_t i = 0; i < cnt_phi; ++i) {
        uint64_t p = pr_phi[i];
        /* divide out this prime factor as much as possible */
        while (order % (seq_t)p == 0) {
            seq_t cand = order / (seq_t)p;
            if (mod_pow(10, (uint64_t)cand, m) == 1) {
                order = cand;
            } else {
                break;
            }
        }
    }
    return order;
}
