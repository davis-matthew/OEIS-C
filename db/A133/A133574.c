/*
 * A133574  Expansion of (5 * phi(q^5)^2 - phi(q)^2) / 4 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A133574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - b(n), where b(n) is multiplicative b(2^e) = 1, b(5^e) =
 *            1 - 4e, b(p^e) = (1 + (-1)^e)/2 for p ≡ 3 (mod 4), b(p^e) = e +
 *            1 for p ≡ 1 (mod 4). a(0) = 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* offset 0: a(0)=1 */

    uint64_t m = (uint64_t)n;             /* work on 64‑bit value */
    seq_t prod = 1;                       /* product for b(n) */

    /* factor 2 – contributes 1, so just strip it */
    while ((m & 1ULL) == 0) m >>= 1;

    /* factor 5 */
    uint64_t e5 = 0;
    while (m % 5ULL == 0) {
        ++e5;
        m /= 5ULL;
    }
    if (e5 > 0) {
        prod *= (seq_t)(1 - (seq_t)4 * (seq_t)e5);
    }

    /* remaining odd prime factors (p ≠ 5) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            if ((p % 4) == 3) {               /* p ≡ 3 (mod 4) */
                if (e & 1) {                  /* odd exponent → factor 0 */
                    prod = 0;
                    break;
                }                              /* even exponent → factor 1 */
            } else if ((p % 4) == 1) {        /* p ≡ 1 (mod 4) */
                prod *= (seq_t)(e + 1);       /* e + 1 */
            }
        }
    }

    if (prod == 0) return (seq_t)0;

    if (m > 1) {                           /* final prime > sqrt(original n) */
        if ((m % 4) == 3) {
            prod = 0;                       /* odd exponent of p ≡ 3 (mod 4) */
        } else {
            prod *= (seq_t)2;                /* exponent is 1 → e + 1 = 2 */
        }
    }

    return -prod;                          /* a(n) = -b(n) */
}
