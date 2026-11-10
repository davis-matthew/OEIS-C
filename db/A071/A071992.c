/*
 * A071992  a(n) = 3*n^2 + 2*n - 4 * Sum_{k=1..n} A003159(k).
 * https://oeis.org/A071992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n^2 + 2*n - 4*Sum_{k=1..n} A003159(k), where A003159
 *            lists the numbers whose binary representation ends in an even
 *            number of zeros; computed by direct enumeration of those numbers
 * Range:     u8 n <= 255, u16 n <= 10241, u32 n <= 10241, u64 n <= 10241,
 *            u128 n <= 10241, big any n
 * Validated: standard, pass, 96 terms through n = 8798 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t count = 0;
    uint64_t m = 0;
    while (count < n) {
        m++;
        uint64_t v = m;
        unsigned z = 0;
        while ((v & 1) == 0) {
            v >>= 1;
            z++;
        }
        if ((z & 1) == 0) {
            s += (seq_t)m;
            count++;
        }
    }
    seq_t nn = (seq_t)n;
    seq_t total = 3 * nn * nn + 2 * nn;
    return total - 4 * s;
}
