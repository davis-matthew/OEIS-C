/*
 * A018434  Divisors of 402.
 * https://oeis.org/A018434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 402 in increasing order (1-indexed)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n) {
    static seq_t divisors[10];
    static size_t divisor_count = 0;
    static int initialized = 0;
    if (!initialized) {
        size_t cnt = 0;
        const idx_t N = 402;
        for (idx_t i = 1; i * i <= N; i++) {
            if (N % i == 0) {
                divisors[cnt++] = (seq_t)i;
                if (i != N / i) {
                    divisors[cnt++] = (seq_t)(N / i);
                }
            }
        }
        qsort(divisors, cnt, sizeof(seq_t), cmp_seq);
        divisor_count = cnt;
        initialized = 1;
    }
    if (n == 0 || (n - 1) >= divisor_count) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
