/*
 * A018578  Divisors of 660.
 * https://oeis.org/A018578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 660 in ascending order a(1) = 1, a(24) =
 *            660. Undefined for n > 24 or n < 1
 * Range:     u8 n <= 18, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   div_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/div_cmp.h"

seq_t a(idx_t n)
{
    /* 660 = 2^2 * 3^1 * 5^1 * 11^1 */
    static seq_t divs[24];
    static bool initialized = false;
    if (!initialized)
    {
        const seq_t twoP[]   = {1, 2, 4};
        const seq_t threeP[] = {1, 3};
        const seq_t fiveP[]  = {1, 5};
        const seq_t elevenP[] = {1, 11};

        size_t idx = 0;
        for (size_t i = 0; i < sizeof(twoP)/sizeof(twoP[0]); ++i)
            for (size_t j = 0; j < sizeof(threeP)/sizeof(threeP[0]); ++j)
                for (size_t k = 0; k < sizeof(fiveP)/sizeof(fiveP[0]); ++k)
                    for (size_t l = 0; l < sizeof(elevenP)/sizeof(elevenP[0]); ++l)
                    {
                        seq_t prod = twoP[i];
                        prod *= threeP[j];
                        prod *= fiveP[k];
                        prod *= elevenP[l];
                        divs[idx++] = prod;
                    }

        qsort(divs, 24, sizeof(seq_t), div_cmp);
        initialized = true;
    }

    if (n < 1 || n > 24)
        return (seq_t)0;
    return divs[(idx_t)(n - 1)];
}
