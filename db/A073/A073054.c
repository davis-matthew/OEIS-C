/*
 * A073054  Number of applications of DENEAT operator x -> A073053(x) needed
 *          to transform n to 123.
 * https://oeis.org/A073054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = number of iterations of x -> A073053(x) (concatenate
 *            #even digits, #odd digits, #digits) needed to reach 123 from n;
 *            a(123) = 0. Computed by direct iteration
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A073054_deneat(uint64_t m)
{
    uint64_t e = 0, o = 0, t = 0;
    if (m == 0) {
        e = 1;
        t = 1;
    } else {
        while (m > 0) {
            unsigned d = (unsigned)(m % 10);
            if ((d & 1u) == 0) e++;
            else o++;
            t++;
            m /= 10;
        }
    }
    uint64_t pt = (t < 10) ? 10 : 100;
    uint64_t po = (o < 10) ? 10 : 100;
    return e * (po * pt) + o * pt + t;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    unsigned steps = 0;
    while (x != 123) {
        x = A073054_deneat(x);
        steps++;
    }
    return (seq_t)steps;
}
