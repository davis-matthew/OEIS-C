/*
 * A018490  Divisors of 504.
 * https://oeis.org/A018490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 504 in ascending order
 * Range:     u8 n <= 22, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *x, const void *y)
{
    seq_t a = *(const seq_t *)x;
    seq_t b = *(const seq_t *)y;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n <= 0 */

    static seq_t divs[24];
    static bool init = false;
    if (!init)
    {
        seq_t pow2[4] = {1, 2, 4, 8};
        seq_t pow3[3] = {1, 3, 9};
        seq_t pow7[2] = {1, 7};

        size_t idx = 0;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    divs[idx++] = pow2[i] * pow3[j] * pow7[k];
                }
            }
        }
        qsort(divs, 24, sizeof(seq_t), cmp_seq);
        init = true;
    }

    if (n > 24) return 0; /* no further terms */
    return divs[n - 1];
}
