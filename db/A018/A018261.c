/*
 * A018261  Divisors of 48.
 * https://oeis.org/A018261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 48 (sorted increasingly)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on seq_t objects */
static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* a(n) returns the n-th divisor of 48 (1-based). */
seq_t a(idx_t n) {
    static seq_t divs[10];
    static int initialized = 0;
    if (!initialized) {
        size_t k = 0;
        seq_t pow2 = 1;
        for (int e2 = 0; e2 <= 4; ++e2) {
            seq_t pow3 = 1;
            for (int e3 = 0; e3 <= 1; ++e3) {
                divs[k++] = pow2 * pow3;
                pow3 *= 3;
            }
            pow2 *= 2;
        }
        qsort(divs, 10, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }
    if (n == 0 || n > 10) return (seq_t)0;
    return divs[n - 1];
}
