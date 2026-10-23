/*
 * A183097  a(n) = sum of powerful divisors d (including 1) of n.
 * https://oeis.org/A183097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^a || n} (1 + Σ_{k=2}^{a} p^k)
 * Range:     u8 n <= 143, u16 n <= 9531, u32 n <= 9531, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       = product over prime powers of a truncated geometric sum   */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    /* factor by trial division; works fine for n up to at least 10⁶ */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp >= 2) {
                seq_t sum = 1;                    /* the '1' term */
                seq_t pow = (seq_t)p * (seq_t)p;  /* p^2 */
                for (int k = 2; k <= exp; ++k) {
                    sum += pow;
                    pow *= p;                    /* raise to next power */
                }
                result *= sum;
            }
        }
    }

    /* any remaining prime factor has exponent 1 -> contributes 1 */
    return result;
}
