/*
 * A120512  a(n) = min{j : A120501(j) = n}.
 * https://oeis.org/A120512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + f(n-1), a(1)=1 where f(k) = v2(k)+1 for k not a
 *            power of 2, and f(k) = v2(k)+3 for k a power of 2. v2(k) is the
 *            exponent of 2 in the prime factorization of k
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t exponent_of_two(idx_t n)
{
    idx_t v = 0;
    while ((n & 1) == 0) {
        ++v;
        n >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=1 */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;          /* a(1) */
    for (idx_t k = 1; k < n; ++k) {
        idx_t orig = k;
        idx_t v = exponent_of_two(orig);          /* v2(k) */
        bool is_pow2 = (orig & (orig - 1)) == 0; /* k is power of 2 ? */

        seq_t inc = (seq_t)v + (seq_t)(is_pow2 ? 3 : 1);
        result += inc;
    }
    return result;
}
