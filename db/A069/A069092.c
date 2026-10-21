/*
 * A069092  Jordan function J_7(n).
 * https://oeis.org/A069092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = n^7 * ∏_{p|n} (1 - 1/p^7) Multiplicative implementation:
 *            a(p^e) = p^(7e) – p^(7(e-1)) (indices start at 1, a(1) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

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

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)(n == 1 ? 1 : 0);

    seq_t result = 1;
    idx_t m = n;

    /* trial division */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* term = p^(7e) – p^(7(e-1)) */
        seq_t term = pow_seq((seq_t)p, 7 * e) - pow_seq((seq_t)p, 7 * (e - 1));
        result *= term;
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        /* m is prime, exponent = 1 */
        seq_t term = pow_seq((seq_t)m, 7) - 1;   /* p^7 – p^0 */
        result *= term;
    }

    return result;
}
