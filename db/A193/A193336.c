/*
 * A193336  Sum of even divisors of sigma(n).
 * https://oeis.org/A193336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all even divisors of sigma(n)
 * Range:     u8 n <= 16, u16 n <= 2098, u32 n <= 8870, u64 n <= 8870,
 *            u128 n <= 8870, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = Σ_{d | σ(n), d even} d */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute σ(n) for 0 ≤ n ≤ UINT64_MAX */
static seq_t sigma_of_uint64(uint64_t x)
{
    if (x <= 1) return (seq_t)x;          /* σ(0)=0, σ(1)=1 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(x, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t e = (seq_t)ex[i];

        seq_t pow = 1;
        for (seq_t k = 0; k < e + 1; ++k) pow *= p;   /* p^(e+1) */

        seq_t term = (pow - 1) / (p - 1);             /* (p^(e+1)‑1)/(p‑1) */
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* σ(n) */
    uint64_t n64 = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64(n64, pr, ex, &cnt);

    seq_t sigma_n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t e = (seq_t)ex[i];

        seq_t pow = 1;
        for (seq_t k = 0; k < e + 1; ++k) pow *= p;

        seq_t term = (pow - 1) / (p - 1);
        sigma_n *= term;
    }

    /* m = σ(n) */
    uint64_t m = (uint64_t)sigma_n;          /* works for all tested ranges */

    seq_t sigma_m   = sigma_of_uint64(m);

    /* odd part of m */
    uint64_t odd = m;
    while ((odd & 1ULL) == 0) odd >>= 1;
    seq_t sigma_odd = sigma_of_uint64(odd);

    return sigma_m - sigma_odd;              /* sum of even divisors of σ(n) */
}
