/*
 * A028594  Expansion of (theta_3(q) * theta_3(q^7) + theta_2(q) *
 *          theta_2(q^7))^2 in powers of q.
 * https://oeis.org/A028594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Π_{p^e||n, p≠7} (p^(e+1)-1)/(p-1), with a(0)=1
 * Range:     u8 n <= 29, u16 n <= 5279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t m = n;
    idx_t p = 2;

    /* trial division up to sqrt(m) */
    while (p <= m / p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p != 7) {
                seq_t sum = 0;
                seq_t term = 1;
                for (idx_t i = 0; i < e; ++i) {
                    sum += term;
                    term *= (seq_t)p;
                }
                sum += term;          /* add the last term p^e */
                prod *= sum;
            }
        }
        ++p;
    }

    /* remaining prime factor (if any) */
    if (m > 1 && m != 7) {
        prod *= ((seq_t)1 + (seq_t)m); /* sum = 1 + p */
    }

    prod *= (seq_t)4;  /* final factor */
    return prod;
}
