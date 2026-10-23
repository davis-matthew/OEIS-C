/*
 * A018506  Divisors of 534.
 * https://oeis.org/A018506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   cmp_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th divisor of 534 in increasing order */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_div.h"

seq_t a(idx_t n) {
    /* sequence is defined for n ≥ 1; return 0 for out of range */
    if (n < 1) return 0;

    static seq_t divs[8];
    static bool init = false;
    if (!init) {
        /* generate all divisors of 534 = 2 * 3 * 89 */
        int idx = 0;
        for (idx_t i = 0; i <= 1; ++i) {
            for (idx_t j = 0; j <= 1; ++j) {
                for (idx_t k = 0; k <= 1; ++k) {
                    seq_t val = 1;
                    if (i) val *= (seq_t)2;
                    if (j) val *= (seq_t)3;
                    if (k) val *= (seq_t)89;
                    divs[idx++] = val;
                }
            }
        }
        /* sort the divisors */
        qsort(divs, 8, sizeof(seq_t), cmp_div);
        init = true;
    }
    /* return the requested element if it exists */
    return (n - 1) < 8 ? divs[n - 1] : (seq_t)0;
}
