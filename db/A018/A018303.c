/*
 * A018303  Divisors of 147.
 * https://oeis.org/A018303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 147 in ascending order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort – works for all arithmetic or big‑int seq_t */
static int compare_seq(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 < v2) ? -1 : ((v1 > v2) ? 1 : 0);
}

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static size_t div_count = 0;
    static bool initialized = false;

    if (!initialized) {
        size_t idx = 0;
        for (idx_t e3 = 0; e3 <= 1; ++e3) {
            seq_t p3 = 1;
            for (idx_t i = 0; i < e3; ++i)
                p3 *= (seq_t)3;
            for (idx_t e7 = 0; e7 <= 2; ++e7) {
                seq_t p7 = 1;
                for (idx_t i = 0; i < e7; ++i)
                    p7 *= (seq_t)7;
                divs[idx++] = p3 * p7;
            }
        }
        div_count = idx;
        qsort(divs, div_count, sizeof(seq_t), compare_seq);
        initialized = true;
    }

    if (n < 1 || n > (idx_t)div_count)
        return (seq_t)0;
    return divs[n - 1];
}
