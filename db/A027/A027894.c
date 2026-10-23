/*
 * A027894  Divisors of 9999.
 * https://oeis.org/A027894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 9999 sorted in ascending order. a(n) returns the nth
   divisor (1‑based).  For n > 12 the sequence is undefined. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* Invalid indices */
    if (n < 1)
        return 0;

    /* Static cache of the 12 divisors */
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        int pos = 0;
        /* Generate all 12 divisors: 3^e3 * 11^e11 * 101^e101
           with e3 in [0,2], e11 in [0,1], e101 in [0,1]  */
        for (int e3 = 0; e3 <= 2; ++e3) {
            seq_t v3 = 1;
            for (int i = 0; i < e3; ++i)
                v3 *= 3;
            for (int e11 = 0; e11 <= 1; ++e11) {
                seq_t v11 = (e11 == 0) ? 1 : 11;
                for (int e101 = 0; e101 <= 1; ++e101) {
                    seq_t v101 = (e101 == 0) ? 1 : 101;
                    divisors[pos++] = v3 * v11 * v101;
                }
            }
        }
        /* Sort the array in ascending order */
        qsort(divisors, 12, sizeof(seq_t), seq_cmp);
        initialized = true;
    }

    if (n > 12)
        return 0;   /* beyond the known range */

    return divisors[n - 1];
}
