/*
 * A187730  Greatest common divisor of Carmichael lambda(n) and n - 1.
 * https://oeis.org/A187730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(λ(n), n-1) (λ is Carmichael’s function)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1, safe fallback */

    /* n-1 for the final gcd */
    seq_t n_minus1 = (seq_t)n - 1;

    /* factor n: arrays up to 64 prime factors are sufficient for all
       64‑bit numbers */
    size_t cnt = 0;
    uint64_t pr[64], ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t lam = 1;                       /* Carmichael lambda of n */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t lam_p;

        if (p == 2) {                    /* special rule for 2^k */
            if (e == 1) lam_p = 1;
            else if (e == 2) lam_p = 2;
            else lam_p = pow_uint(2, e - 2); /* 2^(e-2) */
        } else {                         /* odd prime powers */
            seq_t p_pow = pow_uint(p, e - 1);            /* p^(e-1) */
            lam_p = p_pow * (seq_t)(p - 1);              /* φ(p^e) */
        }

        /* lam = lcm(lam, lam_p) = lam/gcd(lam,lam_p) * lam_p  */
        seq_t g = gcd(lam, lam_p);
        if (lam == 0 || lam_p == 0)
            lam = 0;
        else
            lam = (lam / g) * lam_p;
    }

    return gcd(lam, n_minus1);
}
