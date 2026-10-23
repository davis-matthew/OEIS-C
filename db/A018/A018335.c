/*
 * A018335  Divisors of 208.
 * https://oeis.org/A018335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 208 in increasing order
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[10];
    static size_t divisor_count = 0;
    if (!divisor_count) {
        seq_t N = 208;
        seq_t tmp[20];
        size_t tmpcnt = 0;
        for (idx_t i = 1; (seq_t)i * i <= N; ++i) {
            if (N % (seq_t)i == 0) {
                tmp[tmpcnt++] = (seq_t)i;
                seq_t other = N / (seq_t)i;
                if (other != (seq_t)i) {
                    tmp[tmpcnt++] = other;
                }
            }
        }
        /* insertion sort the temporary array */
        for (size_t i = 1; i < tmpcnt; ++i) {
            seq_t key = tmp[i];
            size_t j = i;
            while (j > 0 && tmp[j - 1] > key) {
                tmp[j] = tmp[j - 1];
                --j;
            }
            tmp[j] = key;
        }
        for (size_t i = 0; i < tmpcnt; ++i) {
            divisors[i] = tmp[i];
        }
        divisor_count = tmpcnt;
    }
    if (n >= 1 && n <= divisor_count) {
        return divisors[n - 1];
    } else {
        return (seq_t)0;
    }
}
