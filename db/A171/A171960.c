/*
 * A171960  Values of the 2-complement of n in ternary representation.
 * https://oeis.org/A171960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3^m - 1 - n, where m = number of base‑3 digits of n (m=1
 *            for n=0)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS A171960 */
seq_t a(idx_t n)
{
    /* Determine the number of digits m in base 3 of n.
       For n = 0 we need m = 1 to match the definition. */
    idx_t tmp = n;
    idx_t m = 1;
    if (tmp > 0) {
        while (tmp >= 3) {
            tmp /= 3;
            ++m;
        }
    }

    /* Compute 3^m in seq_t type. */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < m; ++i) {
        pow3 *= (seq_t)3;
    }

    /* a(n) = 3^m - 1 - n */
    return pow3 - (seq_t)1 - (seq_t)n;
}
