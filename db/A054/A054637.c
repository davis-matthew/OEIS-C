/*
 * A054637  Partial sums of A054635.
 * https://oeis.org/A054637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A054635(k), where A054635 is the base-3
 *            Champernowne digit sequence 0,1,2,1,0,1,1,1,2,... ; computed by
 *            walking base-3 numbers and summing their digits
 * Range:     u8 n <= 238, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 987 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t remaining = (uint64_t)n + 1;   /* digits still to consume */
    uint64_t k = 0;
    seq_t sum = 0;

    while (remaining > 0) {
        unsigned char d[48];
        int len = 0;
        uint64_t t = k;
        if (t == 0) {
            d[len++] = 0;
        } else {
            while (t > 0) {
                d[len++] = (unsigned char)(t % 3);
                t /= 3;
            }
        }
        /* d[len-1] is the most significant digit */
        if ((uint64_t)len <= remaining) {
            unsigned s = 0;
            for (int i = 0; i < len; i++) s += d[i];
            sum += (seq_t)s;
            remaining -= (uint64_t)len;
        } else {
            unsigned s = 0;
            for (uint64_t i = 0; i < remaining; i++) s += d[len - 1 - (int)i];
            sum += (seq_t)s;
            remaining = 0;
        }
        k++;
    }
    return sum;
}
