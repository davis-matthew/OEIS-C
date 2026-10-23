/*
 * A018587  Divisors of 676.
 * https://oeis.org/A018587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 676 in increasing order; a(n)=0 for
 *            n>9
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for offset 0 */

    static seq_t divs[9];
    static int initialized = 0;

    if (!initialized) {
        size_t idx = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t p2 = 1;
            for (int i = 0; i < e2; ++i) p2 *= 2;
            for (int e13 = 0; e13 <= 2; ++e13) {
                seq_t p13 = 1;
                for (int i = 0; i < e13; ++i) p13 *= 13;
                divs[idx++] = p2 * p13;
            }
        }
        qsort(divs, 9, sizeof(seq_t), cmp_seq);
        initialized = 1;
    }

    if (n <= 9)
        return divs[n - 1];
    return 0; /* beyond the last divisor */
}
