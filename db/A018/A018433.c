/*
 * A018433  Divisors of 400.
 * https://oeis.org/A018433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 400
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 400 = 2⁴ · 5²  →  5 × 3 = 15 divisors */
    const idx_t max_pow2 = 4;   /* exponent of 2 */
    const idx_t max_pow5 = 2;   /* exponent of 5 */
    const size_t total = (max_pow2 + 1) * (max_pow5 + 1);
    seq_t divs[total];
    size_t pos = 0;

    seq_t p2 = 1;
    for (idx_t i = 0; i <= max_pow2; ++i) {
        if (i > 0) p2 *= 2;
        seq_t p5 = 1;
        for (idx_t j = 0; j <= max_pow5; ++j) {
            if (j > 0) p5 *= 5;
            divs[pos++] = p2 * p5;
        }
    }

    /* simple ascending sort (15 elements is trivial) */
    for (size_t i = 0; i < total; ++i) {
        for (size_t j = i + 1; j < total; ++j) {
            if (divs[j] < divs[i]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    if (n < 1 || n > total) return 0;
    return divs[n - 1];
}
