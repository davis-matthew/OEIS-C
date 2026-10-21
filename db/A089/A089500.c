/*
 * A089500  Denominators for computation of column sequences of triangle
 *          A071951 (Legendre-Stirling).
 * https://oeis.org/A089500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N(n)/D(n) with * N(n) = sfac(n‑1)*sfac(2n+1)/sfac(n+1) =
 *            ∏_{i=n+2}^{2n+1} i! * D(n) = gcd{ (2m+1)·(m(m+1))^n·N(n) /
 *            ((n+m+1)!·(n−m)!) : 1 ≤ m ≤ n } * * The function below computes
 *            a(n) exactly for all n ≥ 1. It builds a list * of primes up to
 *            2n+1, computes the exponents of each prime in N...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes_upto, exp_in_int, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- utility prototypes ----------------------------------------------------- */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/exp_in_int.h"

/* --- prime exponent in factorial ------------------------------------------------- */
static idx_t exp_fact(idx_t n, uint64_t p)
{
    idx_t exp = 0;
    while (n)
    {
        n /= p;
        exp += n;
    }
    return exp;
}

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* 1. generate list of primes up to 2n+1 ----------------------------------- */
    idx_t limit = 2 * n + 1;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* 2. compute exponent of each prime in N(n) -------------------------------- */
    idx_t *expN = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!expN) { free(primes); return (seq_t)0; }

    for (size_t pj = 0; pj < prime_cnt; ++pj)
    {
        uint64_t p = primes[pj];
        idx_t sum = 0;
        for (idx_t i = n + 2; i <= 2 * n + 1; ++i)
            sum += exp_fact(i, p);
        expN[pj] = sum;
    }

    /* 3. initialise minimum exponent across the list of L_m terms --------------- */
    idx_t *minExp = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!minExp) { free(primes); free(expN); return (seq_t)0; }

    for (size_t pj = 0; pj < prime_cnt; ++pj)
        minExp[pj] = (idx_t)(-1);   /* maximum possible value (wrap‐around) */

    /* 4. iterate over m = 1..n and update minExp for each prime ----------------- */
    for (idx_t m = 1; m <= n; ++m)
    {
        idx_t m_minus = n - m;          /* n-m */
        idx_t m_plus  = n + m + 1;      /* n+m+1 */

        for (size_t pj = 0; pj < prime_cnt; ++pj)
        {
            uint64_t p = primes[pj];

            idx_t e2m1   = exp_in_int(2 * m + 1, p);  /* in (2m+1)             */
            idx_t em     = exp_in_int(m,     p);      /* in m                   */
            idx_t em1    = exp_in_int(m + 1, p);      /* in m+1                 */
            idx_t e_nmp1 = exp_fact(m_plus, p);       /* in (n+m+1)!           */
            idx_t e_nm   = exp_fact(m_minus, p);      /* in (n-m)!             */

            /* exponent of p in one term of the gcd list */
            idx_t expL = e2m1 + n * (em + em1) + expN[pj] - e_nmp1 - e_nm;

            if (expL < minExp[pj])
                minExp[pj] = expL;
        }
    }

    /* 5. assemble the result a(n) = ∏ p^{expN(p) – minExp(p)} ------------------- */
    seq_t result = (seq_t)1;
    for (size_t pj = 0; pj < prime_cnt; ++pj)
    {
        idx_t diff = expN[pj] - minExp[pj];
        if (diff == 0)
            continue;
        seq_t part = seq_pow((seq_t)primes[pj], diff);
        result = seq_mul(result, part);
    }

    /* 6. cleanup --------------------------------------------------------------- */
    free(primes);
    free(expN);
    free(minExp);

    return result;
}
