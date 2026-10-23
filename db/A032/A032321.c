/*
 * A032321  Number of aperiodic necklaces with n labeled beads of 2 colors.
 * https://oeis.org/A032321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{d|n} μ(d) * 2^{n/d} where μ is the Möbius
 *            function
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow2.h"

/* Helper to enumerate divisors that contribute to the Möbius sum. */
static void dfs_collect(idx_t idx, idx_t cur_d, int mu_val, idx_t n_val,
                        const idx_t *primes, const int *exps, int cnt,
                        seq_t *pos, seq_t *neg)
{
    if (idx == cnt) {
        seq_t term = pow2(n_val / cur_d);
        if (mu_val == 1) {
            *pos += term;
        } else if (mu_val == -1) {
            *neg += term;
        }
        return;
    }

    const idx_t prime = primes[idx];
    const int max_e = exps[idx];

    /* Choice: exponent 0 */
    dfs_collect(idx + 1, cur_d, mu_val, n_val,
                primes, exps, cnt, pos, neg);

    /* Choices: exponent 1 or >1 (those with μ=0 are skipped) */
    idx_t val = 1;
    for (int e = 1; e <= max_e; ++e) {
        val *= prime;
        if (e == 1) {
            dfs_collect(idx + 1, cur_d * val, -mu_val, n_val,
                        primes, exps, cnt, pos, neg);
        } else {
            /* exponent > 1 ⇒ μ = 0, so no need to recurse further */
            /* (We could skip recursion because μ stays zero) */
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Step 1: (n-1)! */
    idx_t fact_n_minus1 = n - 1; /* safe because n ≥ 1 */
    seq_t fact = factorial(fact_n_minus1);

    /* Step 2: factorize n */
    idx_t tmp = n;
    idx_t primes[20];  /* enough for n ≤ 10⁴ */
    int   exps [20];
    int   cnt  = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Step 3: compute Σ μ(d) * 2^{n/d} */
    seq_t posSum = 0;
    seq_t negSum = 0;
    dfs_collect(0, 1, 1, n, primes, exps, cnt, &posSum, &negSum);

    /* Result sum is guaranteed non‑negative */
    seq_t sum = posSum - negSum;

    /* Step 4: final product */
    return fact * sum;
}
