/*
 * A018514  Divisors of 550.
 * https://oeis.org/A018514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 550 listed in ascending order */

#include <stdbool.h>
#include <stdlib.h>

static int cmp_seq_t(const void *p, const void *q) {
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t *divs = NULL;
    static size_t div_count = 0;

    if (!initialized) {
        const seq_t N = 550;
        // 550 has only 12 divisors
        seq_t temp[12];
        size_t idx = 0;

        for (idx_t i = 1; (seq_t)i * (seq_t)i <= N; ++i) {
            if (N % (seq_t)i == 0) {
                temp[idx++] = (seq_t)i;
                seq_t other = N / (seq_t)i;
                if (other != (seq_t)i) {
                    temp[idx++] = other;
                }
            }
        }
        div_count = idx;
        divs = (seq_t *)malloc(div_count * sizeof(seq_t));
        for (size_t i = 0; i < div_count; ++i) {
            divs[i] = temp[i];
        }
        qsort(divs, div_count, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0) return (seq_t)0;
    if ((idx_t)(n - 1) < div_count) {
        return divs[n - 1];
    } else {
        return (seq_t)0;
    }
}
