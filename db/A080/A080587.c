/*
 * A080587  Partial sums of A080586.
 * https://oeis.org/A080587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A080586(k); A080586 is blocks of 3*2^j ones
 *            then 3*2^j twos, j=0,1,2,...; each full block j sums to 9*2^j
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9842 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;   /* number of terms to sum */
    seq_t s = 0;
    seq_t p = 1;
    while (1) {
        seq_t half = 3 * p;
        seq_t len = 2 * half;
        if (m >= len) {
            s += 3 * half;
            m -= len;
            p *= 2;
            if (m == 0) return s;
        } else {
            if (m <= half) s += m;
            else s += half + 2 * (m - half);
            return s;
        }
    }
}
