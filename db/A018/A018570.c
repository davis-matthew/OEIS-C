/*
 * A018570  Divisors of 648.
 * https://oeis.org/A018570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 648 in ascending order
 * Range:     u8 n <= 18, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper comparator for uint64_t */
static int cmp_u64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Return the n-th divisor of 648 (offset 1).  For n out of range
   return 0. */
seq_t a(idx_t n)
{
    if (n < 1 || n > 20)
        return 0;

    static seq_t cache[20];
    static bool built = false;

    if (!built)
    {
        /* Compute all divisors of 648 using 2^i * 3^j (i∈[0,3], j∈[0,4]) */
        uint64_t pow2[4];
        uint64_t pow3[5];
        pow2[0] = 1;
        for (int i = 1; i < 4; ++i)
            pow2[i] = pow2[i - 1] * 2;

        pow3[0] = 1;
        for (int j = 1; j < 5; ++j)
            pow3[j] = pow3[j - 1] * 3;

        uint64_t tmp[20];
        size_t idx = 0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 5; ++j)
                tmp[idx++] = pow2[i] * pow3[j];  /* 20 values */

        /* Sort the 64‑bit values first to avoid overflow when casting */
        qsort(tmp, 20, sizeof(uint64_t), cmp_u64);

        /* Convert the sorted values to seq_t */
        for (size_t i = 0; i < 20; ++i)
            cache[i] = (seq_t)tmp[i];

        built = true;
    }

    return cache[n - 1];
}
