/*
 * A018604  Divisors of 711.
 * https://oeis.org/A018604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 711 in ascending order */
#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t N   = 711;            /* fixed number */
    const idx_t LIM = 64;            /* enough for all divisors of 711 */
    seq_t        temp[LIM];
    size_t       count = 0;

    /* Find all divisors of N */
    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            temp[count++] = (seq_t)i;
            if (i != N / i) {
                temp[count++] = (seq_t)(N / i);
            }
        }
    }

    /* Simple bubble sort (count is small, <= 6) */
    for (size_t i = 0; i + 1 < count; ++i) {
        for (size_t j = i + 1; j < count; ++j) {
            if (temp[i] > temp[j]) {
                seq_t t   = temp[i];
                temp[i]   = temp[j];
                temp[j]   = t;
            }
        }
    }

    /* Return nth divisor (1‑based), or 0 if out of bounds */
    if (n == 0 || n > count) {
        return (seq_t)0;
    }
    return temp[n - 1];
}
