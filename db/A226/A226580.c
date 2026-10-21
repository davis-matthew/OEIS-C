/*
 * A226580  Smallest number of integer-sided squares needed to tile a 7 X n
 *          rectangle.
 * https://oeis.org/A226580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(n-7) for n > 8 * initial values: * a(0)=0, a(1)=7,
 *            a(2)=5, a(3)=5, a(4)=5, * a(5)=5, a(6)=5, a(7)=1, a(8)=7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[9] = {0, 7, 5, 5, 5, 5, 5, 1, 7};

seq_t a(idx_t n)
{
    if (n < 9) {
        return base[(size_t)n];
    }
    /* Number of 7‑step reductions needed to reach the base range. */
    idx_t steps = (n - 2) / 7;       /* n>=9 ⇒ steps >= 1 */
    idx_t t = n - 7 * steps;         /* t will be in 0..8 */
    return base[(size_t)t] + (seq_t)steps;
}
