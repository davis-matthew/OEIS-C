/*
 * A054636  Partial sums of A054634.
 * https://oeis.org/A054636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=0..n} A054634(k), where A054634 is the base-8
 *            Champernowne digit sequence 0,1,2,...,7,1,0,1,1,... ; computed
 *            by streaming the base-8 digits of 0,1,2,
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9277 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t remaining = n + 1;   /* number of digits to consume */
    seq_t sum = 0;
    idx_t m = 0;
    while (remaining > 0) {
        unsigned char d[32];
        int len = 0;
        idx_t t = m;
        if (t == 0) {
            d[len++] = 0;
        } else {
            while (t > 0) {
                d[len++] = (unsigned char)(t & 7u);
                t >>= 3;
            }
        }
        /* d[len-1] is the most significant digit */
        for (int i = len - 1; i >= 0 && remaining > 0; --i) {
            sum += (seq_t)d[i];
            --remaining;
        }
        ++m;
    }
    return sum;
}
