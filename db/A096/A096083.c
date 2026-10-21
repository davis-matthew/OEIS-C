/*
 * A096083  Numbers k such that the sum of the squared digits of k plus the
 *          largest digit of k is a square.
 * https://oeis.org/A096083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th non–negative integer k such that (sum of the
 *            squares of the decimal digits of k) + (the largest digit of k)
 *            is a perfect square
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;

    while (true) {
        seq_t temp = k;
        int sumsq = 0;
        int maxd  = 0;

        if (temp == 0) {
            sumsq = 0;
            maxd  = 0;
        } else {
            while (temp != 0) {
                unsigned int d = (unsigned int)(temp % 10);
                sumsq += d * d;
                if ((int)d > maxd) maxd = (int)d;
                temp /= 10;
            }
        }

        int total = sumsq + maxd;

        int s = 0;
        while ((s + 1) * (s + 1) <= total) ++s;
        if (s * s == total) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
