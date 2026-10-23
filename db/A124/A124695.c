/*
 * A124695  Numbers n such that sum of digits of n^2 = sum of digits of
 *          (reverse n)^2.
 * https://oeis.org/A124695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer x (starting at 1) such that
 *            digit_sum(x^2) == digit_sum((reverse_decimal(x))^2)
 * Range:     u8 n <= 12, u16 n <= 67, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   digit_sum, reverse_decimal, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/reverse_decimal.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index starts at 1, value 0 at n=1 */
    idx_t idx = 0;                  /* count of found numbers */
    seq_t x = 0;                    /* candidate integer */

    while (1) {
        seq_t sq = seq_mul(x, x);
        seq_t rev = reverse_decimal(x);
        seq_t sq_rev = seq_mul(rev, rev);

        if (digit_sum(sq) == digit_sum(sq_rev)) {
            ++idx;
            if (idx == n) return x;
        }
        ++x;                          /* next candidate */
    }
}
