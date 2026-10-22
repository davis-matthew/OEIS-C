/*
 * A003553  Divisors of 2^48 - 1.
 * https://oeis.org/A003553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 2^48 - 1, in ascending order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 147, u64 n <= 768, u128 n <= 768,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t divisors_list[2000];
static int div_count = -1;

static int cmp_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x > y) - (x < y);
}

static void compute_divisors(void) {
    if (div_count >= 0) return;
    
    uint64_t N = (1ULL << 48) - 1;
    div_count = 0;
    
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            divisors_list[div_count++] = (seq_t)d;
            if (d * d != N) {
                divisors_list[div_count++] = (seq_t)(N / d);
            }
        }
    }
    
    qsort(divisors_list, (size_t)div_count, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n) {
    compute_divisors();
    if (n > 0 && n <= (idx_t)div_count) {
        return divisors_list[n - 1];
    }
    return 0;
}
