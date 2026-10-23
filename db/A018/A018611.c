/*
 * A018611  Divisors of 724.
 * https://oeis.org/A018611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 724, sorted increasingly (1-indexed)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Lazy static cache of the divisors of 724 */
    static seq_t *divs = NULL;
    static idx_t divcnt = 0;

    if (divcnt == 0) {
        /* 724 = 2^2 * 181 has at most 6 divisors */
        uint64_t tmp[6];
        idx_t c = 0;

        /* Naïve enumeration of all divisors of 724 */
        for (uint64_t d = 1; d <= 724; ++d) {
            if (724 % d == 0) {
                tmp[c++] = d;
            }
        }

        /* Sort the divisors (simple insertion sort, at most 6 elements) */
        for (idx_t i = 1; i < c; ++i) {
            uint64_t key = tmp[i];
            idx_t j = i - 1;
            while (j + 1 > 0 && tmp[j] > key) {
                tmp[j + 1] = tmp[j];
                --j;
            }
            tmp[j + 1] = key;
        }

        divcnt = c;
        divs = (seq_t *)malloc(divcnt * sizeof(seq_t));
        for (idx_t i = 0; i < divcnt; ++i) {
            divs[i] = (seq_t)tmp[i];
        }
    }

    if (n == 0 || n > divcnt) {
        return (seq_t)0;
    }
    return divs[n - 1];
}
