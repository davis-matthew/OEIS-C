/*
 * A078430  Sum of gcd(k^2,n) for 1 <= k <= n.
 * https://oeis.org/A078430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏_{p^e||n} f(p,e) with f(p,e) = p^(3e/2) + p^(3e/2-1) -
 *            p^(e-1) if e even = 2*p^((3e-1)/2) - p^(e-1) if e odd
 * Range:     u8 n <= 35, u16 n <= 1295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast power for arbitrary‑precision seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* trial division factorisation for 64‑bit n.
   arrays are at most 64 entries, sufficient for n up to 2^64. */
static void factor_u64(idx_t n, uint64_t *pr, uint64_t *exp, idx_t *cnt)
{
    idx_t c = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (m > 1) {
        pr[c] = m;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* only required for safety */

    uint64_t pr[64];
    uint64_t expv[64];
    idx_t cnt;

    factor_u64(n, pr, expv, &cnt);

    seq_t result = 1;

    for (idx_t i = 0; i < cnt; ++i) {
        uint64_t p  = pr[i];
        idx_t    e  = expv[i];
        seq_t term;

        if ((e & 1) == 0) {          /* e even: e = 2m */
            idx_t m = e >> 1;
            seq_t p3m   = pow_seq((seq_t)p, 3 * m);
            seq_t p3m1  = pow_seq((seq_t)p, 3 * m - 1);
            seq_t pe1   = pow_seq((seq_t)p, e - 1);
            term = p3m + p3m1 - pe1;
        } else {                     /* e odd: e = 2m+1 */
            idx_t m = (e - 1) >> 1;
            seq_t p3m1 = pow_seq((seq_t)p, 3 * m + 1);
            seq_t p2m   = pow_seq((seq_t)p, 2 * m);
            term = 2 * p3m1 - p2m;
        }

        result *= term;
    }

    return result;
}
