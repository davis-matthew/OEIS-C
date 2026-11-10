/*
 * A086869  a(n) = floor(abs(1/sin(A070751(n)))).
 * https://oeis.org/A086869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(abs(1/sin(m))) where m = A070751(n) is the n-th
 *            positive integer with sin(m) < 0; computed by a direct scan with
 *            long double sin
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    unsigned long long m = 1;

    for (;; ++m) {
        long double s = sinl((long double)m);
        if (s < 0.0L) {
            ++count;
            if (count == n) {
                long double v = floorl(1.0L / (-s));
                return (seq_t)(unsigned long long)v;
            }
        }
    }
}
