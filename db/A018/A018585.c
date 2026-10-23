/*
 * A018585  Divisors of 672.
 * https://oeis.org/A018585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 672, sorted in ascending order. */

#include <stdlib.h>
#include <stdbool.h>

/* The value 672 = 2^5 * 3 * 7.  It has exactly 24 divisors. */
seq_t a(idx_t n) {
    static seq_t divisors[24];
    static bool initialized = false;

    if (!initialized) {
        /* Generate all divisors 2^a * 3^b * 7^c with 0<=a<=5, 0<=b<=1, 0<=c<=1 */
        int idx = 0;
        for (int a = 0; a <= 5; ++a) {
            for (int b = 0; b <= 1; ++b) {
                for (int c = 0; c <= 1; ++c) {
                    seq_t val = 1;
                    for (int i = 0; i < a; ++i) val *= (seq_t)2;
                    if (b) val *= (seq_t)3;
                    if (c) val *= (seq_t)7;
                    divisors[idx++] = val;
                }
            }
        }

        /* Simple insertion sort (24 elements is trivial). */
        for (size_t i = 1; i < 24; ++i) {
            seq_t temp = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > temp) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = temp;
        }

        initialized = true;
    }

    if (n >= 1 && n <= 24) {
        return divisors[(size_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
