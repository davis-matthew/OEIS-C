/*
 * A091986  a(0)=1, a(n) = sigma_3(2n).
 * https://oeis.org/A091986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_3(2*n) (sum of cubes of the divisors of 2*n)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   factor_u64, pow_seq_t_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt) */
#include "../utils/pow_seq_t_uint64.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) return 1;

    uint64_t m = 2ULL * (uint64_t)n;          /* m = 2*n */

    /* factor m */
    uint64_t primes[32];
    uint64_t exps  [32];
    size_t   cnt   = 0U;
    factor_u64(m, primes, exps, &cnt);

    seq_t sigma = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* sum_{k=0..e} p^{3k} */
        seq_t pk3       = pow_seq_t_uint64(p, 3ULL);   /* p^3 */
        seq_t pk_pow    = 1;                           /* p^{3k} with k=0 */
        seq_t term_sum  = 1;                           /* include k=0 term */

        for (uint64_t k = 0; k < e; ++k) {
            pk_pow *= pk3;      /* p^{3*(k+1)} */
            term_sum += pk_pow;
        }

        sigma *= term_sum;
    }

    return sigma;
}
