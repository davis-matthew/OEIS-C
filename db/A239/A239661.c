/*
 * A239661  Divisors of 1089.
 * https://oeis.org/A239661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 1089 (33^2) in ascending order */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[9];
    static size_t count = 0;

    if (count == 0) {
        seq_t pow3 = 1;
        size_t idx = 0;

        for (int i = 0; i <= 2; ++i) {
            seq_t pow11 = 1;
            for (int j = 0; j <= 2; ++j) {
                seq_t val = pow3 * pow11;

                /* insert val into sorted array */
                size_t pos = idx;
                while (pos > 0 && divisors[pos - 1] > val) {
                    divisors[pos] = divisors[pos - 1];
                    --pos;
                }
                divisors[pos] = val;
                ++idx;

                pow11 *= (seq_t)11;
            }
            pow3 *= (seq_t)3;
        }
        count = idx;
    }

    if (n == 0 || n > count)
        return 0;
    return divisors[n - 1];
}
