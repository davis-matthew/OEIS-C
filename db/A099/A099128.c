/*
 * A099128  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3,4,5,6,7,8,9}.
 * https://oeis.org/A099128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n + 9, n) + n - 1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t first  = binomial(n + 9, n);
    seq_t inner  = first + (seq_t)n - 1;
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = inner - (seq_t)n + (seq_t)i;
        result = seq_mul(result, factor);
        result = result / (seq_t)i;
    }

    return result;
}
