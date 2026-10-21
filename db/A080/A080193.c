/*
 * A080193  5-smooth numbers which are not 3-smooth.
 * https://oeis.org/A080193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5 * A051037(n)
 * Range:     u8 n <= 24, u16 n <= 189, u32 n <= 1499, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation uses the standard three‑pointer method for 5‑smooth numbers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* unused, offset starts at 1 */
    idx_t size = n;                          /* we need n elements, 1‑based */
    seq_t *h = (seq_t *)malloc((size + 1) * sizeof(seq_t));
    if (!h) return 0;                        /* allocation failure, unlikely */

    /* 5‑smooth numbers: numbers with prime factors only 2, 3, 5. */
    h[0] = 1;                                 /* first such number */
    idx_t i2 = 0, i3 = 0, i5 = 0;             /* indices for next multiples */

    for (idx_t i = 1; i < size; ++i) {
        seq_t v2 = h[i2] * 2;
        seq_t v3 = h[i3] * 3;
        seq_t v5 = h[i5] * 5;
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;

        h[i] = next;

        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
    }

    seq_t result = h[size - 1] * 5;
    free(h);
    return result;
}
