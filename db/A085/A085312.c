/*
 * A085312  Number of distinct 9th powers modulo n.
 * https://oeis.org/A085312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of distinct 9th powers modulo n (multiplicative;
 *            computed via prime factorization)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t total = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t f;
            if (p == 2) {
                seq_t phi = ((seq_t)1 << (exp - 1)); /* φ(2^e) = 2^(e-1) */
                f = 1 + phi;
            } else if (p == 3) {
                seq_t phi = pow_uint(3, exp - 1) * (seq_t)2; /* φ(3^e) = 3^(e-1)*2 */
                seq_t g = (exp >= 3) ? 9 : (exp == 2 ? 3 : 1);
                f = 1 + phi / g;
            } else {
                seq_t phi = pow_uint(p, exp - 1) * (seq_t)(p - 1); /* φ(p^e) = p^(e-1)*(p-1) */
                seq_t g = (seq_t)gcd_uint64(9, (uint64_t)phi);
                f = 1 + phi / g;
            }
            total *= f;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        idx_t p = temp;
        seq_t f;
        if (p == 2) {
            seq_t phi = ((seq_t)1 << (1 - 1)); /* φ(2^1) = 1 */
            f = 1 + phi;
        } else if (p == 3) {
            seq_t phi = (seq_t)2;       /* φ(3^1) = 2 */
            f = 1 + phi;                /* gcd(9,2)=1 */
        } else {
            seq_t phi = (seq_t)(p - 1); /* φ(p^1) = p-1 */
            seq_t g = (seq_t)gcd_uint64(9, (uint64_t)phi);
            f = 1 + phi / g;
        }
        total *= f;
    }
    return total;
}
