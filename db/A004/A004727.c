/*
 * A004727  Delete all 8's from the sequence of nonnegative integers.
 * https://oeis.org/A004727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   search: for k = 0, 1, 2, ... delete every digit 8 from k; skip k
 *            if nothing is left (all digits are 8), else the remaining digits
 *            give the next term. a(n) is the n-th term produced
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    uint64_t k = 0;
    idx_t count = 0;
    for (;;) {
        uint64_t m = k;
        seq_t result = 0;
        seq_t power = 1;
        bool any = false;
        if (m == 0) {
            any = true;
        }
        while (m > 0) {
            uint64_t d = m % 10;
            if (d != 8) {
                result += (seq_t)d * power;
                power *= 10;
                any = true;
            }
            m /= 10;
        }
        if (any) {
            if (count == n) {
                return result;
            }
            count++;
        }
        k++;
    }
}
