/*
 * A018289  Divisors of 112.
 * https://oeis.org/A018289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 112
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t divs[10];
    static bool inited = false;
    if (!inited) {
        size_t idx = 0;
        for (int e2 = 0; e2 <= 4; ++e2) {
            seq_t pow2 = 1;
            for (int i = 0; i < e2; ++i) pow2 *= 2;
            for (int e7 = 0; e7 <= 1; ++e7) {
                seq_t val = pow2;
                if (e7) val *= 7;
                divs[idx++] = val;
            }
        }
        qsort(divs, 10, sizeof(seq_t), cmp_seq);
        inited = true;
    }
    if (n == 0 || n > 10) return 0;
    return divs[n - 1];
}
