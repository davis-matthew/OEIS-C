/*
 * A033109  Number of 8's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{b=2}^{n+1} (number of digits equal to 8 in the base‑b
 *            representation of n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 8 no base between 2 and n+1 can represent the digit 8 */
    if (n < 8) {
        return (seq_t)0;
    }

    seq_t total = (seq_t)0;
    const idx_t last_base = n + 1;

    for (idx_t base = 2; base <= last_base; ++base) {
        uint64_t temp = (uint64_t)n;            /* use a 64‑bit holder for division */
        const uint64_t ub = (uint64_t)base;     /* promote base for modulo/division */

        while (temp > 0) {
            if ((temp % ub) == 8) {
                total += (seq_t)1;
            }
            temp /= ub;
        }
    }

    return total;
}
