/*
 * A189180  Number of nX3 array permutations with each element making a single
 *          king move.
 * https://oeis.org/A189180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A189180: Number of n×3 array permutations with each element making a single king move.
   Implemented via the linear recurrence (order 8) derived in the OEIS entry:
     a(n) = 8*a(n-1) + 94*a(n-2) - 88*a(n-3) - 488*a(n-4)
            + 152*a(n-5) - 638*a(n-6) - 72*a(n-7) + 9*a(n-8)
        with a(1)=0, a(2)=33, a(3)=192, a(4)=4081, a(5)=47936,
        a(6)=733465, a(7)=9925824, a(8)=142150545.
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* not part of the sequence */
    if (n <= 8) {
        static const seq_t init[8] = {
            (seq_t)0ULL,
            (seq_t)33ULL,
            (seq_t)192ULL,
            (seq_t)4081ULL,
            (seq_t)47936ULL,
            (seq_t)733465ULL,
            (seq_t)9925824ULL,
            (seq_t)142150545ULL
        };
        return init[n - 1];
    }

    /* sliding window for the last 8 terms: v[0]=a(k-8), ..., v[7]=a(k-1) */
    seq_t v[8] = {
        (seq_t)0ULL,
        (seq_t)33ULL,
        (seq_t)192ULL,
        (seq_t)4081ULL,
        (seq_t)47936ULL,
        (seq_t)733465ULL,
        (seq_t)9925824ULL,
        (seq_t)142150545ULL
    };

    for (idx_t k = 9; k <= n; ++k) {
        /* compute the next term using only additions and subtractions
           of unsigned values (negative part subtracted after the positive part) */
        seq_t pos = (seq_t)8   * v[7] +  // 8*a(n-1)
                    (seq_t)94  * v[6] +  // 94*a(n-2)
                    (seq_t)152 * v[3] +  // 152*a(n-5)
                    (seq_t)9   * v[0];   // 9*a(n-8)

        seq_t neg = (seq_t)88  * v[5] +  // 88*a(n-3)
                    (seq_t)488 * v[4] +  // 488*a(n-4)
                    (seq_t)638 * v[2] +  // 638*a(n-6)
                    (seq_t)72  * v[1];   // 72*a(n-7)

        seq_t next = pos - neg;          // guaranteed non‑negative

        /* shift window left by one position, drop the oldest term */
        for (int j = 0; j < 7; ++j) v[j] = v[j + 1];
        v[7] = next;
    }

    return v[7];   /* a(n) */
}
