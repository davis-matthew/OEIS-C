/*
 * A092350  a(n) = sigma_3(n) - sigma_2(n) - sigma_1(n).
 * https://oeis.org/A092350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma_3(n) - sigma_2(n) - sigma_1(n)
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 1223, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 7200 (2026-11-01)
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
    if (n < 1)                  /* index below the offset is undefined */
        return 0;

    seq_t s3 = sigma_k(n, 3);   /* σ₃(n)  */
    seq_t s2 = sigma_k(n, 2);   /* σ₂(n)  */
    seq_t s1 = sigma_k(n, 1);   /* σ₁(n)  */

    return s3 - s2 - s1;
}
