/*
 * A193581  Sort-and-subtract: a(n) = n - A004185(n).
 * https://oeis.org/A193581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n - sorted_digits_of_n(n)
 * Range:     u8 n <= 289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    size_t cnt[10] = {0};

    seq_t tmp = n_val;
    if (tmp == 0) {
        cnt[0] = 1;
    } else {
        while (tmp > 0) {
            seq_t d = tmp % 10;   /* d is 0..9 */
            cnt[(unsigned)d]++;
            tmp /= 10;
        }
    }

    seq_t sorted = 0;
    for (int d = 0; d < 10; ++d) {
        for (size_t k = 0; k < cnt[d]; ++k) {
            sorted = sorted * 10 + (seq_t)d;
        }
    }

    return n_val - sorted;
}
