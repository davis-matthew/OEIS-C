/*
 * A092592  a(n) = A001142(n)/A002944(n), i.e., the product of C(n,j) binomial
 *          coefficients (for j=0..n) is divided by the least common multiple
 *          of them.
 * https://oeis.org/A092592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{j=0}^{n} C(n,j) / lcm_{j=0}^{n} C(n,j)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* build_primes(limit,&out,&cnt)   */
#include "../utils/seq_pow.h"       /* seq_pow(base,exp)              */
#include "../utils/seq_mul.h"       /* seq_mul(a,b)                   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* Allocate/reuse array for v_p(k!) up to current n */
    static idx_t *vfact = NULL;
    static size_t vfact_cap = 0;
    if (n > vfact_cap) {
        free(vfact);
        vfact = (idx_t *)malloc((n + 1) * sizeof(idx_t));
        vfact_cap = n;
    }

    /* Get list of primes <= n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    /* For each prime compute exponent contribution */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];

        /* vfact[k] = v_p(k!) for 0..n; also accumulate sum_j v_p(j!) */
        idx_t sum_fact = 0;
        vfact[0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            idx_t temp = k;
            idx_t mult = 0;
            while (temp % p == 0) {
                temp /= p;
                ++mult;
            }
            vfact[k] = vfact[k-1] + mult;
            sum_fact += vfact[k];
        }

        idx_t vp_nfact = vfact[n];

        /* sum over j of v_p(C(n,j)) */
        idx_t sum_vc = (idx_t)((n + 1) * (idx_t)vp_nfact - 2 * (idx_t)sum_fact);

        /* maximum exponent of p in any C(n,j) */
        idx_t max_vc = 0;
        for (idx_t j = 0; j <= n; ++j) {
            idx_t val = vp_nfact - vfact[j] - vfact[n - j];
            if (val > max_vc) max_vc = val;
        }

        idx_t exp = sum_vc - max_vc;
        if (exp > 0) {
            seq_t powp = seq_pow((seq_t)p, exp);
            result = seq_mul(result, powp);
        }
    }

    free(primes);
    return result;
}
