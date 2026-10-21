/*
 * A094519  Numbers having at least one pair (x,y) of divisors with x<y such
 *          that x+y is also a divisor.
 * https://oeis.org/A094519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest integer m ≥ 6 such that * there exist
 *            divisors x < y of m with (x + y) also dividing m
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether m satisfies the property.  m is even. */
static bool has_property(idx_t m)
{
    /* store all divisors of m in an array */
    seq_t divs[128];
    size_t cnt = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (((seq_t)m) % (seq_t)d == 0) {
            divs[cnt++] = (seq_t)d;
            if (d * d != m)
                divs[cnt++] = (seq_t)(m / d);
        }
    }

    /* search for a pair (x, y), x < y, with x + y | m */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t x = divs[i];
        for (size_t j = i + 1; j < cnt; ++j) {
            seq_t y  = divs[j];
            seq_t sum = x + y;
            /* sum cannot be zero for positive divisors */
            if (((seq_t)m) % sum == 0)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 4;                /* start before the first even candidate */
    for (;; m += 2) {           /* test only even numbers */
        if (has_property(m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
