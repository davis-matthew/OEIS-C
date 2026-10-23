/*
 * A029934  Denominator of n * Product_{d|n} (1 + 1/d).
 * https://oeis.org/A029934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of n*∏_{d|n}(1+1/d)
 * Range:     u8 n <= 29, u16 n <= 71, u32 n <= 143, u64 n <= 359,
 *            u128 n <= 1439, big any n
 * Helpers:   build_divs, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine factors n, generates all its divisors, and
 *  computes the prime‑exponent balance between the product of all
 *  divisors (∏d) and the product n·∏(d+1).  The reduced denominator
 *  is then assembled from the remaining positive exponents.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/build_divs.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not defined, guards */

    /* --- factorise n ----------------------------------------------- */
    uint64_t fac_pr[32];
    uint64_t fac_ex[32];
    size_t fac_cnt;
    factor_u64((uint64_t)n, fac_pr, fac_ex, &fac_cnt);

    /* τ(n) = number of divisors */
    idx_t tau = 1;
    for (size_t i = 0; i < fac_cnt; ++i) tau *= (idx_t)(fac_ex[i] + 1);

    /* --- list all divisors ------------------------------------------ */
    size_t max_div = (size_t)tau;               /* τ(n) is small for n≤10000 */
    uint64_t *divs = (uint64_t*)malloc(max_div * sizeof(uint64_t));
    size_t div_cnt = 0;
    build_divs(0, 1ULL, fac_pr, fac_ex, fac_cnt, divs, &div_cnt);

    /* --- exponents in D and N --------------------------------------- */
    idx_t *expD = (idx_t*)malloc(fac_cnt * sizeof(idx_t));
    idx_t *expN = (idx_t*)malloc(fac_cnt * sizeof(idx_t));
    for (size_t i = 0; i < fac_cnt; ++i) {
        expD[i] = (idx_t)fac_ex[i] * tau / 2;   /* exponent in ∏d   */
        expN[i] = (idx_t)fac_ex[i];             /* exponent from n   */
    }

    /* sum v_p(d+1) over all divisors d of n -------------------------- */
    for (size_t di = 0; di < div_cnt; ++di) {
        uint64_t val = divs[di] + 1ULL;
        for (size_t pi = 0; pi < fac_cnt; ++pi) {
            uint64_t p   = fac_pr[pi];
            idx_t    cnt = 0;
            while (val % p == 0) {
                val /= p;
                ++cnt;
            }
            expN[pi] += cnt;
        }
    }

    /* --- reduced denominator ---------------------------------------- */
    seq_t denom = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        if (expD[i] > expN[i]) {
            idx_t e   = expD[i] - expN[i];
            seq_t pwr = seq_pow((seq_t)fac_pr[i], e);
            denom     = seq_mul(denom, pwr);
        }
    }

    free(divs);
    free(expD);
    free(expN);
    return denom;
}
