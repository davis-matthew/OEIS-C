/*
 * A061875  |First digit - second digit + third digit - fourth digit ...| = 6.
 * https://oeis.org/A061875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers with |first digit - second digit + third digit - fourth digit ...| = 6  */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        /* extract decimal digits */
        int digits[40];
        int len = 0;
        idx_t t = x;
        while (t > 0) {
            digits[len++] = t % 10;
            t /= 10;
        }
        /* alternating sum starting with positive on the most‑significant digit */
        int sum = 0;
        for (int i = 0; i < len; ++i) {
            int d = digits[len - 1 - i];
            sum += ((i & 1) == 0) ? d : -d;
        }
        if (sum < 0) sum = -sum;
        if (sum == 6) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
