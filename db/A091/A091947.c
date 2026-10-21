/*
 * A091947  (Fractional part of 1.1^n) * 10^n.
 * https://oeis.org/A091947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11^n mod 10^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include <stdbool.h> /* for external linkage; no direct use */

seq_t a(idx_t n)
{
    seq_t base = 11;               /* base 11 */
    seq_t mod  = pow10(n);         /* modulus 10^n */
    seq_t result = 1;
    idx_t exp = n;

    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result % mod;           /* handles n == 0 correctly */
}
