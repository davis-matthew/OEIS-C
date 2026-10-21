/*
 * A057521  Powerful part of n: if n = Product_i (pi^ei) then a(n) =
 *          Product_{i : ei > 1} (pi^ei); if n = b*c^2*d^3 then a(n) = c^2*d^3
 *          when b is minimized.
 * https://oeis.org/A057521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of all prime powers p^e from the factorisation of
 *            n such that e > 1 (the powerful part of n). Example: a(40) = 2^3
 *            = 8
 * Range:     u8 n <= 255, u16 n <= 9247, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   my_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed */
#include "../utils/my_pow_uint.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1.  For n = 1, return 1. */
    seq_t res = 1;
    if (n <= 1) return res;

    uint64_t m = (uint64_t)n;          /* work with a 64‑bit integer */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp > 1) {
                res *= my_pow_uint(p, (idx_t)exp);
            }
        }
    }
    /* After the loop, if m > 1 then m is a prime factor with exponent 1
       (no contribution to the powerful part).  Thus nothing else to do. */

    return res;
}
