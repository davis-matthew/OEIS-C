/*
 * A120890  Ordered odd leg of primitive Pythagorean triangles.
 * https://oeis.org/A120890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: The nth term is the nth odd number that can be written as
   m² – n² with gcd(m,n)=1 and m>n of opposite parity.  Each odd number
   occurs as many times as it has coprime odd factor pairs.  We generate
   odd values in order, count their multiplicity, and stop once the
   cumulative count reaches n. */

#include "../utils/gcd.h"

static idx_t multiplicity_of_odd(idx_t x)
{
    idx_t cnt = 0;
    for (idx_t d = 1; d * d <= x; d += 2) {
        if (x % d == 0) {
            idx_t e = x / d;
            if (d < e && (d & 1) && (e & 1)) {
                if (gcd((seq_t)d, (seq_t)e) == 1)
                    ++cnt;
            }
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t val = 3;
    while (1) {
        idx_t mult = multiplicity_of_odd((idx_t)val);
        if (seen + mult >= n)
            return val;
        seen += mult;
        val += 2;
    }
}
