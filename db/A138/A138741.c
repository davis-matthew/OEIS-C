/*
 * A138741  Expansion of q^(-1/2) * eta(q)^3 * eta(q^4) * eta(q^12) /
 *          (eta(q^2)^2 * eta(q^3)) in powers of q (unsigned).
 * https://oeis.org/A138741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product over the odd prime powers of (2*n+1) with the
 *            multiplicative rule: b(3^e) = 3 if e odd, 1 if e even; b(p^e) =
 *            e+1 if p ≡ 1 or 5 (mod 12); b(p^e) = 0 if p ≡ 7 or 11 (mod 12)
 *            and e is odd, else 1 if e even; a(n) = b(2*n+1). This implements
 *            the OEIS A138741 sequence
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* m = 2*n + 1 (always odd) */
    uint64_t m = (uint64_t)(((unsigned __int128)2 * n) + 1);

    seq_t result = 1;
    uint64_t temp = m;

    /* Factorization by trial division over odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            /* count exponent */
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t term;
            if (p == 3) {
                term = (exp & 1) ? (seq_t)3 : (seq_t)1;
            } else if ((p % 12 == 1) || (p % 12 == 5)) {
                term = (seq_t)(exp + 1);
            } else if ((p % 12 == 7) || (p % 12 == 11)) {
                term = (exp & 1) ? (seq_t)0 : (seq_t)1;
            } else {
                /* This case never occurs for odd primes except 3 */
                term = 1;
            }
            if (term == 0) return 0;
            result *= term;
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        uint64_t p = temp;      /* exponent is 1 */
        seq_t term;
        if (p == 3) {
            term = 3;
        } else if ((p % 12 == 1) || (p % 12 == 5)) {
            term = 2;
        } else if ((p % 12 == 7) || (p % 12 == 11)) {
            term = 0;
        } else {
            term = 1;
        }
        if (term == 0) return 0;
        result *= term;
    }

    return result;
}
