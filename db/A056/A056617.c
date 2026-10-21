/*
 * A056617  Denominator of binomial(2*n,n) / (2*n+1).
 * https://oeis.org/A056617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of binomial(2*n, n) / (2*n+1) Computed as
 *            a(n) = (2*n+1) / gcd( binomial(2*n,n), 2*n+1 )
 * Range:     u8 n <= 127, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void factor_uint64(uint64_t m, uint32_t *pr, uint8_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint8_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[c] = (uint32_t)p;
            exp[c] = e;
            ++c;
        }
    }
    if (m > 1) { pr[c] = (uint32_t)m; exp[c] = 1; ++c; }
    *cnt = c;
}

static uint64_t fact_exp(uint64_t n, uint64_t p)
{
    uint64_t cnt = 0;
    while (n) {
        n /= p;
        cnt += n;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Compute m = 2*n + 1; we assume it fits in uint64_t. */
    unsigned __int128 t = (unsigned __int128)n << 1;
    t += 1;
    uint64_t m = (uint64_t)t;   /* m <= UINT64_MAX by assumption */

    /* Factor m.  Because m <= 2*n+1, its prime factors are small. */
    uint32_t pr[64];
    uint8_t  pe[64];
    size_t   pc;
    factor_uint64(m, pr, pe, &pc);

    /* Compute the gcd = product of prime factors to the min(power in m,
       power in binomial(2n,n)). */
    uint64_t gcd = 1;
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = pr[i];
        uint64_t e_m = pe[i];
        uint64_t e_cn = fact_exp(m - 1, p) - 2 * fact_exp(n, p); /* v_p(Cn) */
        uint64_t use = e_m < e_cn ? e_m : e_cn;
        uint64_t pow = 1;
        for (uint64_t k = 0; k < use; ++k) pow *= p;
        gcd *= pow;
    }

    uint64_t denom = m / gcd;
    return (seq_t)denom;
}
