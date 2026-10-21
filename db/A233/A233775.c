/*
 * A233775  Number of vertices in the n-th row of the Sierpinski gasket (cf.
 *          A047999).
 * https://oeis.org/A233775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (2^t(n) + 1) * 2^(c(n) - 1) where t(n) = number of
 *            trailing zeros in the binary representation of n, and c(n) =
 *            number of ones (population count) in the binary representation
 *            of n
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0: a(0) = 1 by definition. */
    if (n == 0)
        return (seq_t)1;

    /* Compute t(n) – trailing zeros – and c(n) – popcount. */
    idx_t t = __builtin_ctzll(n);          /* trailing zeros of n (n > 0) */
    idx_t c = __builtin_popcountll(n);     /* number of set bits in n */

    /* Use 64‑bit arithmetic for the intermediate result.
       For n up to 10000 the value fits in 64 bits – the same holds for
       the 64‑bit build.  Larger seq_t types (128‑bit, arbitrary precision)
       automatically accept the casted value. */
    uint64_t tmp = ((uint64_t)1 << t) + 1;    /* 2^t + 1 */
    tmp <<= (c - 1);                          /* multiply by 2^(c-1) */

    return (seq_t)tmp;
}
