/*
 * A018609  Divisors of 720.
 * https://oeis.org/A018609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   cmp_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 720: a(n) = n-th divisor of 720 in ascending order (1‑based) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/cmp_divs.h"

static seq_t divs[30];
static size_t div_count = 0;
static int divs_initialized = 0;

static void init_divs(void) {
    if (divs_initialized) return;
    size_t idx = 0;
    for (int i = 0; i <= 4; ++i) {
        seq_t val2 = 1;
        for (int t = 0; t < i; ++t) val2 *= 2;
        for (int j = 0; j <= 2; ++j) {
            seq_t val3 = val2;
            for (int t = 0; t < j; ++t) val3 *= 3;
            for (int k = 0; k <= 1; ++k) {
                seq_t val = val3;
                for (int t = 0; t < k; ++t) val *= 5;
                divs[idx++] = val;
            }
        }
    }
    div_count = idx;        /* should be 30 */
    qsort(divs, div_count, sizeof(seq_t), cmp_divs);
    divs_initialized = 1;
}

seq_t a(idx_t n) {
    init_divs();
    if (n >= 1 && (idx_t)n <= div_count) {
        return divs[(size_t)(n - 1)];
    }
    return 0; /* undefined for n > number of divisors */
}
