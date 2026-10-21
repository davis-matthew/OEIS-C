/*
 * A061765  a(n) = usigma(sigma(n)), where usigma(n) is the sum of unitary
 *          divisors of n (A034448) and sigma(n) is the sum of the divisors
 *          (A000203).
 * https://oeis.org/A061765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034448(A000203(n))
 * Range:     u8 n <= 16, u16 n <= 5110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"
#include "../utils/pow_int_seq.h"

/* ---------- Sum of divisors (sigma) -------------------------------- */
static seq_t sigma_seq(idx_t n)
{
    uint64_t u = (uint64_t)n;
    uint64_t fac[20];
    int exp[20];
    int cnt;

    factor_uint64(u, fac, exp, &cnt);

    seq_t result = 1;
    for (int i = 0; i < cnt; ++i)
    {
        /* term = (p^(e+1) - 1) / (p - 1) */
        seq_t p_pow = pow_int_seq((seq_t)fac[i], (idx_t)(exp[i] + 1));
        seq_t term  = (p_pow - 1) / ((seq_t)(fac[i] - 1));
        result *= term;
    }
    return result;
}

/* ---------- Sum of unitary divisors (usigma) ------------------------ */
static seq_t usigma_uint64(uint64_t m)
{
    if (m == 1)
        return 1;                       /* usigma(1) = 1 */

    uint64_t fac[20];
    int exp[20];
    int cnt;

    factor_uint64(m, fac, exp, &cnt);

    seq_t result = 1;
    for (int i = 0; i < cnt; ++i)
    {
        seq_t p_pow = pow_int_seq((seq_t)fac[i], (idx_t)exp[i]);
        seq_t term  = 1 + p_pow;
        result *= term;
    }
    return result;
}

/* ---------------- Main sequence ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1, but guard anyway */
        return 0;

    seq_t sigma_n = sigma_seq(n);

    /* For n up to 10 000 the value of sigma(n) fits in 64 bits. */
    uint64_t sigma_ll = (uint64_t)sigma_n;

    return usigma_uint64(sigma_ll);
}
