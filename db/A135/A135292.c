/*
 * A135292  a(n) = 5^n * 4^(n^2).
 * https://oeis.org/A135292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * 4^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 5^n */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* n^2 as an idx_t to avoid overflow of seq_t */
    idx_t exp = n * n;
    seq_t pow4 = big_pow((seq_t)4, (seq_t)exp);

    return seq_mul(pow5, pow4);
}
