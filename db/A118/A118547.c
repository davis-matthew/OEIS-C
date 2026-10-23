/*
 * A118547  Squares which are divisible by the sum of their digits.
 * https://oeis.org/A118547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 73, u32 n <= 9386, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A118547: the nth square that is divisible by the sum of its digits. */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t i = 1;
    idx_t count = 0;

    for (;; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;          /* square of i */
        seq_t ds = digit_sum(sq);                /* sum of digits of the square */
        if (ds == 0) continue;                  /* safety check, should never happen */
        if ((sq % ds) == 0) {                   /* divisible by its digit sum? */
            ++count;
            if (count == n)
                return sq;                      /* found the nth term */
        }
    }
}
