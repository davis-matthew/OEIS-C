/*
 * A090599  Number of n-element labeled commutative groupoids with an
 *          identity.
 * https://oeis.org/A090599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(1 + binomial(n,2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t pow_seq_t_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* exponent = 1 + n*(n-1)/2  */
    idx_t exp = 1 + n * (n - 1) / 2;
    seq_t base = (seq_t)n;
    return pow_seq_t_local(base, exp);
}
