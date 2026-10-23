/*
 * A018406  Divisors of 350.
 * https://oeis.org/A018406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 350 (ascending)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   div_compare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/div_compare.h"

seq_t a(idx_t n)
{
    static seq_t divisors[12];
    static int initialized = 0;

    if (!initialized)
    {
        int idx = 0;
        for (int i = 0; i <= 1; ++i)
        {
            seq_t base_i = (i == 0) ? 1 : (seq_t)2;
            for (int j = 0; j <= 2; ++j)
            {
                seq_t val_ij = base_i;
                for (int t = 0; t < j; ++t) val_ij *= (seq_t)5;
                for (int k = 0; k <= 1; ++k)
                {
                    seq_t val = val_ij;
                    if (k) val *= (seq_t)7;
                    divisors[idx++] = val;
                }
            }
        }
        qsort(divisors, 12, sizeof(seq_t), div_compare);
        initialized = 1;
    }

    if (n >= 1 && n <= 12) return divisors[n - 1];
    return 0;
}
