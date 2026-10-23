/*
 * A018517  Divisors of 556.
 * https://oeis.org/A018517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 556 in increasing order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Comparator for seq_t values. */
static int cmp_seq_t_func(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t divs[64];
    static size_t divcnt = 0;
    static bool init = false;

    if (!init) {
        init = true;
        const idx_t m = 556;           /* number whose divisors we list */
        for (idx_t i = 1; i * i <= m; ++i) {
            if (m % i == 0) {
                divs[divcnt++] = (seq_t)i;
                if (i != m / i)
                    divs[divcnt++] = (seq_t)(m / i);
            }
        }
        qsort(divs, divcnt, sizeof(seq_t), cmp_seq_t_func);
    }

    if (n == 0) return 0;                       /* invalid offset, safe guard */
    if ((idx_t)(n - 1) < divcnt)
        return divs[n - 1];
    return 0;                                   /* n larger than number of divisors */
}
