/*
 * A067817  a(n) = Sum_{r|n, s|n, t|n, r<s<t} r*s*t.
 * https://oeis.org/A067817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sigma_1(n)^3 - 3*sigma_1(n)*sigma_2(n) + 2*sigma_3(n))/6
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 1079, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9506 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t s1 = sigma_k(n, 1);
    seq_t s2 = sigma_k(n, 2);
    seq_t s3 = sigma_k(n, 3);

    seq_t term1 = s1 * s1 * s1;          /* s1^3 */
    seq_t term2 = 2 * s3;                /* 2*s3 */
    seq_t sum   = term1 + term2;         /* s1^3 + 2*s3 */
    seq_t term3 = 3 * s1 * s2;           /* 3*s1*s2 */
    seq_t num   = sum - term3;           /* numerator, guaranteed non‑negative */
    seq_t res   = num / 6;               /* division by 6, integer */

    return res;
}
