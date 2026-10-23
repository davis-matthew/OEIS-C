/*
 * A018426  Divisors of 385.
 * https://oeis.org/A018426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th divisor of 385 in increasing order (enumeration) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* static cache for the divisors of 385 */
    static bool initialized = false;
    static seq_t divisors[8];
    static size_t divcnt = 0;

    if (!initialized) {
        initialized = true;

        const seq_t target = (seq_t)385;
        size_t cnt = 0;
        /* iterate up to sqrt(target) */
        idx_t i = 1;
        for (; (i * i) <= (idx_t)target; ++i) {
            if ((seq_t)i == 0) continue;           /* safety, not needed here */
            if (target % (seq_t)i == 0) {
                divisors[cnt++] = (seq_t)i;
                seq_t d2 = target / (seq_t)i;
                if (d2 != (seq_t)i) {
                    divisors[cnt++] = d2;
                }
            }
        }
        /* sort the found divisors */
        if (cnt > 1) {
            qsort(divisors, cnt, sizeof(seq_t), seq_cmp);
        }
        divcnt = cnt;
    }

    if (n == 0 || (size_t)(n - 1) >= divcnt)
        return (seq_t)0;
    return divisors[n - 1];
}
