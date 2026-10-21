/*
 * A230643  Number of integers m such that m + (sum of digits in base-3
 *          representation of m) = 2n.
 * https://oeis.org/A230643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Count of integers m such that m + sum_of_digits_base3(m) = 2n
 * Range:     u8 n <= 336, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* δ(n) = |{ m : 0 ≤ m ≤ 2n, m + s_3(m) = 2n }| */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * 2;       /* 2n as seq_t */
    seq_t count   = 0;
    idx_t upper   = 2 * n;             /* 2n as idx_t for loop bound */

    for (idx_t m = 0; m <= upper; ++m) {
        seq_t sum = 0;
        idx_t t  = m;
        while (t) {                     /* sum of base‑3 digits of m */
            sum += t % 3;
            t   /= 3;
        }
        if ((seq_t)m + sum == target)
            ++count;
    }
    return count;
}
