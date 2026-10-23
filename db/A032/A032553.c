/*
 * A032553  Arrange digits of cubes in ascending order.
 * https://oeis.org/A032553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digits of n^3 arranged ascending, leading zeros discarded
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t cube = n_t * n_t;
    cube *= n_t;
    if (cube == 0)
        return 0;

    unsigned int counts[10] = {0};
    seq_t temp = cube;
    while (temp > 0) {
        unsigned int digit = (unsigned int)(temp % 10);
        counts[digit]++;
        temp /= 10;
    }

    seq_t result = 0;
    bool leading = true;
    for (int d = 0; d < 10; ++d) {
        unsigned int cnt = counts[d];
        for (unsigned int i = 0; i < cnt; ++i) {
            if (leading && d == 0)
                continue;
            if (leading)
                leading = false;
            result = result * 10 + d;
        }
    }
    return result;
}
