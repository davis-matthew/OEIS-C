/*
 * A053508  a(n) = binomial(n-1,3)*n^(n-4).
 * https://oeis.org/A053508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-1,3) * n^(n-4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* binomial(n-1,3) */
    seq_t bin = binom(n - 1, 3);

    /* n^(n-4) */
    seq_t pw = pow_uint((uint64_t)n, n - 4);

    return bin * pw;
}
