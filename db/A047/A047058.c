/*
 * A047058  a(n) = 6^n * n!.
 * https://oeis.org/A047058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uintptr_t etc., not strictly required but harmless */

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Iteratively compute 6^n * n! as product_{k=1}^n (6*k) */
    for (idx_t k = 1; k <= n; ++k) {
        result = result * (seq_t)k * (seq_t)6;
    }
    return result;
}
