/*
 * A007426  d_4(n), or tau_4(n), the number of ordered factorizations of n as
 *          n = rstu.
 * https://oeis.org/A007426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product_i binomial(3+e_i, 3) where n = Product p_i^e_i
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9680 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_3_plus_e(idx_t e)
{
    seq_t result = (seq_t)(1 + e);
    result *= (seq_t)(2 + e);
    result *= (seq_t)(3 + e);
    result /= 6;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    for (idx_t p = 2; p * p <= temp; p++) {
        idx_t e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            result *= binom_3_plus_e(e);
        }
    }
    
    if (temp > 1) {
        result *= binom_3_plus_e(1);
    }
    
    return result;
}
