/*
 * A018461  Divisors of 448.
 * https://oeis.org/A018461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 448 in ascending order; a(n) = 0 for n>14
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *p, const void *q) {
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n) {
    if (n < 1)           /* Not defined for n < 1, return 0 */
        return 0;

    static seq_t divisors[14];
    static bool initialized = false;

    if (!initialized) {
        seq_t pow2[7];
        seq_t pow7[2];

        pow2[0] = 1;
        for (size_t i = 1; i < 7; ++i)
            pow2[i] = pow2[i - 1] * 2;

        pow7[0] = 1;
        pow7[1] = 7;

        size_t idx = 0;
        for (size_t i = 0; i < 7; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                divisors[idx++] = pow2[i] * pow7[j];
            }
        }
        qsort(divisors, 14, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    if (n > 14)
        return 0;

    return divisors[(size_t)(n - 1)];
}
