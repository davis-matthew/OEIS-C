/*
 * A151672  G.f.: Product_{k>=0} (1 + 4*x^(3^k)).
 * https://oeis.org/A151672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^m if the base‑3 representation of n contains only 0 or
 *            1 digits, where m is the number of 1s in that representation;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 39, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t temp = n;
    int count = 0;

    while (temp != 0) {
        idx_t d = temp % 3;
        if (d == (idx_t)2)          /* digit 2 → impossible representation */
            return (seq_t)0;
        if (d == (idx_t)1)
            ++count;
        temp /= 3;
    }

    for (int i = 0; i < count; ++i)
        result = result * (seq_t)4;   /* multiply by 4^count */

    return result;
}
