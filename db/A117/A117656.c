/*
 * A117656  Number of solutions to x^(k+2)=x^2 mod n for some k>=1.
 * https://oeis.org/A117656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = prod_{p^e||n} (p^⌊e/2⌋ + (p-1)*p^(e-1))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t tmp = n;
    seq_t result = 1;

    /* factorise by trial division */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            seq_t p_val = (seq_t)p;
            seq_t part1 = pow_int(p_val, e / 2);
            seq_t part2 = (seq_t)(p - 1) * pow_int(p_val, e - 1);
            result *= (part1 + part2);
        }
    }

    /* handle remaining prime factor > 1 */
    if (tmp > 1) {
        idx_t p = tmp;
        seq_t p_val = (seq_t)p;
        /* exponent e = 1 */
        seq_t part1 = pow_int(p_val, 0);          /* 1 */
        seq_t part2 = (seq_t)(p - 1) * pow_int(p_val, 0); /* p-1 */
        result *= (part1 + part2);               /* equals p */
    }

    return result;
}
