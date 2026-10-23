/*
 * A168239  Iterate the map n -> sum of largest digit of all divisors of n;
 *          sequence gives number of steps to reach 14.
 * https://oeis.org/A168239
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative map: a(n) = number of steps to reach 14 using
   f(m) = Σ_{d|m} (largest digit of d)   (stop when current = 14) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* small helper – largest decimal digit of a positive integer */
static uint8_t largest_digit_uint64(uint64_t x)
{
    uint8_t max = 0;
    while (x) {
        uint8_t d = x % 10;
        if (d > max) max = d;
        x /= 10;
    }
    return max;
}

/* compute a(n) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n == 14) return 0;
    seq_t steps = 0;
    idx_t cur = n;

    while (cur != 14) {
        ++steps;
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= cur; ++d) {
            if (cur % d == 0) {
                sum += (seq_t)largest_digit_uint64(d);
                idx_t e = cur / d;
                if (e != d)
                    sum += (seq_t)largest_digit_uint64(e);
            }
        }
        cur = (idx_t)sum;
    }
    return steps;
}
