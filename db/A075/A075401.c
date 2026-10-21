/*
 * A075401  Smallest nontrivial multiple of n (> n and not equal to 10^k*n)
 *          whose digit sum is a multiple of the digit sum of n.
 * https://oeis.org/A075401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest nontrivial multiple of n (> n and not
 *            10^k*n) whose digit sum is a multiple of the digit sum of n
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_sum, is_power_of_ten
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs an exhaustive search over multiples k of n
   (k ≥ 2), skipping k that are powers of ten, and returns the first
   multiple whose digit sum is divisible by the digit sum of n. */
#include "../utils/digit_sum.h"
#include <stdbool.h>
#include "../utils/is_power_of_ten.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t s_n   = digit_sum(n_seq);

    for (idx_t k = 2; ; ++k) {
        if (is_power_of_ten(k))
            continue;

        seq_t m = n_seq * (seq_t)k;
        seq_t s_m = digit_sum(m);

        /* digit sum of m is a multiple of digit sum of n */
        if (s_m % s_n == 0)
            return m;
    }
}
