/*
 * A022525  Nexus numbers (n+1)^9-n^9.
 * https://oeis.org/A022525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^9 - n^9
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 11, u64 n <= 194,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow9(seq_t base)
{
    seq_t result = 1;
    for (int i = 0; i < 9; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* compute (n+1)^9 and n^9 in seq_t */
    seq_t n_plus_one = (seq_t)(n + 1);
    seq_t n_pow = pow9((seq_t)n);
    seq_t np1_pow = pow9(n_plus_one);
    return np1_pow - n_pow;
}
