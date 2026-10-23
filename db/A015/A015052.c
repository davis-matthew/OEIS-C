/*
 * A015052  a(n) is the smallest positive integer m such that m^5 is divisible
 *          by n.
 * https://oeis.org/A015052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = the smallest positive integer m such that m^5 is
 *            divisible by n. For n = ∏ p_i^{e_i} → a(n) = ∏
 *            p_i^{ceil(e_i/5)}. (Multiplicative.)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t base, int exp); */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;
    idx_t p = 2;

    /* factorization by trial division */
    while (p <= temp / p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* ceil(exp/5) */
            int needed = (exp + 4) / 5;
            seq_t pow = pow_seq((seq_t)p, needed);
            result *= pow;
        }
        /* skip even numbers after 2 */
        if (p == 2) p = 3;
        else p += 2;
    }

    /* if a prime factor > sqrt(original n) remains */
    if (temp > 1) {
        /* exponent is 1 → ceil(1/5) = 1 */
        seq_t pow = pow_seq((seq_t)temp, 1);
        result *= pow;
    }

    return result;
}
