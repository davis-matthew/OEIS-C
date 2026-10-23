/*
 * A018758  Divisors of 984.
 * https://oeis.org/A018758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 984 in ascending order
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration for qsort comparator. */
static int compare_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Divisors of 984 = 2^3 * 3^1 * 41^1 */
    static seq_t divisors[16];
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        /* Powers of the prime factors. */
        const seq_t pow2[4]  = {1, 2, 4, 8};
        const seq_t pow3[2]  = {1, 3};
        const seq_t pow41[2] = {1, 41};

        idx_t idx = 0;
        for (idx_t i0 = 0; i0 < 4; ++i0)
            for (idx_t i1 = 0; i1 < 2; ++i1)
                for (idx_t i2 = 0; i2 < 2; ++i2)
                    divisors[idx++] = pow2[i0] * pow3[i1] * pow41[i2];

        /* Sort the divisors in ascending order. */
        qsort(divisors, 16, sizeof(seq_t), compare_seq);
    }

    if (n >= 1 && n <= 16)
        return divisors[n - 1];
    return (seq_t)0;  /* Undefined for n > 16 */
}
