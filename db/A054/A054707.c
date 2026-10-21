/*
 * A054707  Number of powers of 6 modulo n.
 * https://oeis.org/A054707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of distinct residues of powers of 6 modulo n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A007737(n) + A244417(n) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not in domain, but guard */
    idx_t tmp = n;
    uint64_t a_exp = 0, b_exp = 0;

    /* factor out powers of 2 */
    while ((tmp & 1u) == 0u) {
        ++a_exp;
        tmp >>= 1;
    }

    /* factor out powers of 3 */
    while (tmp % 3u == 0u) {
        ++b_exp;
        tmp /= 3u;
    }

    uint64_t m = (uint64_t)tmp;               /* part coprime to 6   */
    uint64_t T = (a_exp > b_exp) ? a_exp : b_exp;  /* max exponent of 2 or 3 */

    if (m == 1u) {            /* no coprime part */
        return (seq_t)(T + 1);
    }

    /* m > 1: compute multiplicative order of 6 modulo m */
    seq_t phi_m = phi((idx_t)m);
    uint64_t phi_ul = (uint64_t)phi_m;

    /* factor phi(m) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi_ul, primes, exps, &cnt);

    uint64_t ord = phi_ul;
    seq_t m_seq = (seq_t)m;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        while (ord % p == 0u) {
            if (mod_pow((seq_t)6, ord / p, m_seq) == (seq_t)1)
                ord /= p;
            else
                break;
        }
    }

    return (seq_t)(T + ord);
}
