/*
 * A058766  a(0) = 1, a(1) = 2; for n>=2 a(n) is the number of degree-n
 *          reducible polynomials over GF(2).
 * https://oeis.org/A058766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - (1/n) * Σ_{d|n} μ(d) * 2^(n/d) for n ≥ 2, a(0) = 1,
 *            a(1) = 2 where μ(d) is the Möbius function
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Möbius function μ(d) for d ≥ 1 */
static int mu_of(idx_t d)
{
    if (d == 1) return 1;
    int cnt = 0;
    idx_t n = d;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
                if (exp > 1) return 0;   /* square factor → μ = 0 */
            }
            ++cnt;                       /* distinct prime factor counted */
        }
    }
    if (n > 1) ++cnt;                  /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

seq_t a(idx_t n)
{
    /* Base cases from the OEIS entry */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Allocate array for powers of two up to 2^n */
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow2) return (seq_t)0;        /* allocation failure (unlikely) */

    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* Compute Σ μ(d)·2^(n/d) over the divisors of n */
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            int mu = mu_of(d);
            if (mu == 0) continue;
            seq_t term = pow2[n / d];
            if (mu > 0) sum += term;
            else        sum -= term;
        }
    }

    /* Irreducible monic polynomials of degree n over GF(2) */
    seq_t irreducible = sum / (seq_t)n;     /* division is exact by theory */

    /* Desired number of reducible monic polynomials of degree n over GF(2) */
    seq_t reducible = pow2[n] - irreducible;

    free(pow2);                            /* clean up */
    return reducible;
}
