/*
 * A018586  Divisors of 675.
 * https://oeis.org/A018586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth divisor of 675 in increasing order. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 675 = 3^3 · 5^2; there are 4 × 3 = 12 divisors */
    if (n == 0)
        return 0;

    /* pre‑computed prime powers (small enough for all seq_t variants) */
    static const seq_t pow3[4] = {1, 3, 9, 27};
    static const seq_t pow5[3] = {1, 5, 25};

    seq_t vals[12];
    size_t cnt = 0;

    /* generate all 12 products 3^i · 5^j */
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 3; ++j)
            vals[cnt++] = pow3[i] * pow5[j];

    /* insertion sort – trivial for size 12 */
    for (size_t i = 1; i < 12; ++i) {
        seq_t key = vals[i];
        size_t j = i;
        while (j > 0 && vals[j - 1] > key) {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    if (n > 12)
        return 0;            /* undefined beyond 12th divisor */

    return vals[n - 1];
}
