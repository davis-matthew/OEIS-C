/*
 * A033718  Product theta3(q^d); d | 5.
 * https://oeis.org/A033718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+(-1)^t) * ∏ b(p^e), where b(p^e) = * • (e+1) if p ≡
 *            1,3,7,9 (mod 20) * • 1 if p = 5 * • 0 if p ≡ 11,13,17,19 (mod
 *            20) and e odd * t is the total multiplicity of primes ≡ 2,3,7
 *            (mod 20). a(0)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t t = 0;        /* parity exponent counter */
    seq_t prod = (seq_t)1;

    idx_t temp = n;

    /* factor 2 */
    idx_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e) {
        t += e;          /* 2 contributes to t */
        /* local factor for 2 is 1 → nothing to do */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p) continue;

        e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        idx_t mod20 = p % 20;
        if (mod20 == 2 || mod20 == 3 || mod20 == 7)
            t += e;

        if (mod20 == 5) {
            /* local factor 1 – nothing to do */
        } else if (mod20 == 11 || mod20 == 13 || mod20 == 17 || mod20 == 19) {
            if (e & 1) return (seq_t)0; /* odd exponent gives zero */
        } else { /* mod20 == 1,3,7,9 */
            prod *= (seq_t)(e + 1);
        }
    }

    /* remaining prime factor > sqrt(original) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t mod20 = p % 20;
        /* exponent is one */
        if (mod20 == 2 || mod20 == 3 || mod20 == 7)
            t += 1;

        if (mod20 == 5) {
            /* local factor 1 – nothing to do */
        } else if (mod20 == 11 || mod20 == 13 || mod20 == 17 || mod20 == 19) {
            /* odd exponent 1 → zero */
            return (seq_t)0;
        } else { /* mod20 == 1,3,7,9 */
            prod *= (seq_t)2;     /* e + 1 = 1 + 1 = 2 */
        }
    }

    if (t & 1U)
        return (seq_t)0;          /* factor (1+(-1)^t) is zero */

    return prod * (seq_t)2;       /* multiply by 2 */
}
