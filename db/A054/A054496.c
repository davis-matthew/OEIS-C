/*
 * A054496  If n = p_1^e_1 * p_2^e_2 * p_3^e_3 * ..., p's = distinct primes,
 *          e's = positive integers, then a(n) = p_1^(e_1^2) * p_2^(e_2^2) *
 *          p_3^(e_3^2) * ... .
 * https://oeis.org/A054496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} p^(e^2) (product over prime factors p of n
 *            with exponent e)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base case n == 1 */
    if (n <= 1)
        return 1;

    seq_t result = 1;
    idx_t rem = n;

    /* Factor 2 separately to avoid skipping it in the odd loop */
    if (rem % 2 == 0) {
        int exp = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            exp++;
        }
        idx_t exp2 = (idx_t)exp * (idx_t)exp;          /* e^2 */
        seq_t term = 1;
        seq_t base = 2;
        idx_t e = exp2;
        while (e > 0) {
            if (e & 1) term *= base;
            base *= base;
            e >>= 1;
        }
        result *= term;
    }

    /* Trial division for odd primes */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                exp++;
            }
            idx_t exp2 = (idx_t)exp * (idx_t)exp;      /* e^2 */
            seq_t term = 1;
            seq_t base = p;
            idx_t e = exp2;
            while (e > 0) {
                if (e & 1) term *= base;
                base *= base;
                e >>= 1;
            }
            result *= term;
        }
    }

    /* If remaining factor > 1, it's a prime with exponent 1 (e^2 = 1) */
    if (rem > 1)
        result *= rem;

    return result;
}
