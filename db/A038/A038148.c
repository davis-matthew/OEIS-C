/*
 * A038148  Number of 3-infinitary divisors of n: if n = Product p(i)^r(i) and
 *          d = Product p(i)^s(i), each s(i) has a digit a <= b in its ternary
 *          expansion everywhere that the corresponding r(i) has a digit b,
 *          then d is a 3-infinitary-divisor of n.
 * https://oeis.org/A038148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} ∏_{d in base‑3 digits of e} (d+1) This uses the
 *            multiplicative property of 3‑infinitary divisors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ternary_contrib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ternary_contrib.h"

/* Sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trial division factorisation – sufficiently fast for n up to ~1e6 */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= ternary_contrib(exp);
        }
    }
    if (m > 1) {           /* remaining prime factor */
        result *= ternary_contrib(1);
    }
    return result;
}
