/*
 * A018596  Divisors of 693.
 * https://oeis.org/A018596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 693 (1-indexed)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort when sorting seq_t values */
static int cmp_seq_t(const void *va, const void *vb)
{
    const seq_t *a = (const seq_t *)va;
    const seq_t *b = (const seq_t *)vb;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Precomputed divisors of 693 = 3^2 * 7 * 11. */
    static seq_t divs[12];
    static size_t div_cnt = 0;

    if (div_cnt == 0)
    {
        /* Generate all divisors by iterating over exponents. */
        for (uint64_t e3 = 0; e3 <= 2; ++e3)
        {
            seq_t d3 = 1;
            for (uint32_t k = 0; k < e3; ++k) d3 *= 3;
            for (uint64_t e7 = 0; e7 <= 1; ++e7)
            {
                seq_t d7 = d3;
                for (uint32_t k = 0; k < e7; ++k) d7 *= 7;
                for (uint64_t e11 = 0; e11 <= 1; ++e11)
                {
                    seq_t d = d7;
                    for (uint32_t k = 0; k < e11; ++k) d *= 11;
                    divs[div_cnt++] = d;
                }
            }
        }
        /* Sort the divisors in increasing order. */
        qsort(divs, div_cnt, sizeof(seq_t), cmp_seq_t);
    }

    if (n < 1 || n > div_cnt) return 0;  /* out of bounds */

    return divs[n - 1];
}
