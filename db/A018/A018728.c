/*
 * A018728  Divisors of 931.
 * https://oeis.org/A018728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_uint_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of all divisors of 931 in increasing order
   (OEIS A018728). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_uint_cmp.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence starts at offset 1 */

    /* Cache the divisors so we compute them only once. */
    static seq_t divisors[6];
    static bool init = false;

    if (!init)
    {
        int m = 0;
        const seq_t p7  = (seq_t)7;
        const seq_t p19 = (seq_t)19;

        /* Generate 7^e1 * 19^e2 for e1∈{0,1,2}, e2∈{0,1}. */
        for (int e1 = 0; e1 <= 2; ++e1)
        {
            seq_t pow7 = (seq_t)1;
            for (int i = 0; i < e1; ++i) pow7 *= p7;

            for (int e2 = 0; e2 <= 1; ++e2)
            {
                seq_t pow19 = (seq_t)1;
                for (int i = 0; i < e2; ++i) pow19 *= p19;

                divisors[m++] = pow7 * pow19;
            }
        }

        /* Sort the six values. */
        qsort(divisors, m, sizeof(seq_t), seq_uint_cmp);
        init = true;
    }

    /* Return the n‑th divisor (1‑based). */
    if (n - 1 < 6)
        return divisors[n - 1];
    else
        return (seq_t)0;  /* undefined for indices beyond the divisor count */
}
