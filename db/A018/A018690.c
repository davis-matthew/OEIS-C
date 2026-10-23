/*
 * A018690  Divisors of 864.
 * https://oeis.org/A018690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 864 in increasing order (864 = 2^5 * 3^3) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t divs[24];
static bool init = false;

static int cmp_seq(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (!init) {
        size_t i = 0;
        seq_t pow2 = (seq_t)1;
        for (unsigned e2 = 0; e2 <= 5; ++e2) {
            seq_t pow3 = (seq_t)1;
            for (unsigned e3 = 0; e3 <= 3; ++e3) {
                divs[i++] = pow2 * pow3;
                pow3 = pow3 * (seq_t)3;
            }
            pow2 = pow2 * (seq_t)2;
        }
        qsort(divs, i, sizeof(seq_t), cmp_seq);
        init = true;
    }
    if (n <= 24) {
        return divs[(size_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
