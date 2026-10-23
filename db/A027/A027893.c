/*
 * A027893  Divisors of 99999.
 * https://oeis.org/A027893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 99999 in increasing order
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Build the 12 divisors of 99999 only once */
    static seq_t *divs = NULL;
    static size_t div_cnt = 0;
    static int initialized = 0;

    if (!initialized)
    {
        const seq_t p3[3]   = {1, (seq_t)3,  (seq_t)9};   /* 3^0,3^1,3^2 */
        const seq_t p41[2]  = {1, (seq_t)41};              /* 41^0,41^1 */
        const seq_t p271[2] = {1, (seq_t)271};             /* 271^0,271^1 */

        div_cnt = 12;
        divs = (seq_t *)malloc(div_cnt * sizeof(seq_t));
        size_t idx = 0;
        for (int e3 = 0; e3 <= 2; ++e3)
            for (int e41 = 0; e41 <= 1; ++e41)
                for (int e271 = 0; e271 <= 1; ++e271)
                    divs[idx++] = p3[e3] * p41[e41] * p271[e271];

        /* Sort the 12 numbers in ascending order – simple selection sort */
        for (size_t i = 0; i + 1 < div_cnt; ++i)
        {
            size_t min_idx = i;
            for (size_t j = i + 1; j < div_cnt; ++j)
                if (divs[j] < divs[min_idx]) min_idx = j;
            if (min_idx != i)
            {
                seq_t tmp = divs[i];
                divs[i]   = divs[min_idx];
                divs[min_idx] = tmp;
            }
        }
        initialized = 1;
    }

    /* Offset is 1 – return 0 for indices outside the range */
    if (n == 0) return 0;
    if ((idx_t)n > div_cnt) return 0;
    return divs[n - 1];
}
