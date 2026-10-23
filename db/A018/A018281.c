/*
 * A018281  Divisors of 98.
 * https://oeis.org/A018281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 98
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Generate all divisors of 98 (2 × 7²). */
    seq_t divs[6];
    size_t cnt = 0;

    for (unsigned e2 = 0; e2 <= 1; ++e2) {
        seq_t part2 = e2 ? (seq_t)2 : (seq_t)1;
        for (unsigned e7 = 0; e7 <= 2; ++e7) {
            seq_t part7 = (seq_t)1;
            for (unsigned k = 0; k < e7; ++k)
                part7 *= 7;
            divs[cnt++] = part2 * part7;
        }
    }

    /* Sort the small array of six values. */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    if ((idx_t)n <= cnt)
        return divs[(size_t)n - 1];
    return 0;
}
