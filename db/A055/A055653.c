/*
 * A055653  Sum of phi(d) [A000010] over all unitary divisors d of n (that is,
 *          gcd(d,n/d) = 1).
 * https://oeis.org/A055653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p^e – p^(e-1)) (OEIS A055653)
 * Range:     u8 n <= 255, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t tmp   = (uint64_t)n;

    /* --- factor 2 separately --- */
    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) { tmp >>= 1; ++e; }
        seq_t p = 2;
        seq_t pow_e = 1;
        for (int i = 0; i < e; ++i) pow_e *= p;   /* p^e   */
        seq_t pow_e_minus_one = pow_e / p;         /* p^(e-1) */
        result *= (1 + pow_e - pow_e_minus_one);
    }

    /* --- trial division for odd primes --- */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            int e = 0;
            while (tmp % d == 0) { tmp /= d; ++e; }
            seq_t p = (seq_t)d;
            seq_t pow_e = 1;
            for (int i = 0; i < e; ++i) pow_e *= p;   /* p^e   */
            seq_t pow_e_minus_one = pow_e / p;         /* p^(e-1) */
            result *= (1 + pow_e - pow_e_minus_one);
        }
    }

    /* --- remaining prime factor (if any) --- */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;        /* prime with exponent 1 */
        seq_t pow_e = p;             /* p^1   */
        seq_t pow_e_minus_one = 1;   /* p^0   */
        result *= (1 + pow_e - pow_e_minus_one);
    }

    return result;
}
