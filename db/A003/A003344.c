/*
 * A003344  Numbers that are the sum of 10 positive 4th powers.
 * https://oeis.org/A003344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- mask[s] bit k set iff s is a sum of exactly k positive
 *            4th powers (unbounded knapsack over b^4); a(n) = n-th s with bit
 *            10 set
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *A3344_terms = NULL;
static idx_t A3344_cnt = 0;

static void A3344_build(long long L) {
    uint16_t *mask = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    mask[0] = 1;
    for (long long b = 1; b * b * b * b <= L; b++) {
        long long p = b * b * b * b;
        for (long long s = p; s <= L; s++) {
            mask[s] |= (uint16_t)((mask[s - p] << 1) & 0x7FF);
        }
    }
    idx_t cnt = 0;
    for (long long s = 0; s <= L; s++)
        if (mask[s] & 0x400) cnt++;
    free(A3344_terms);
    A3344_terms = (long long *)malloc((size_t)(cnt + 1) * sizeof(long long));
    idx_t i = 0;
    for (long long s = 0; s <= L; s++)
        if (mask[s] & 0x400) A3344_terms[i++] = s;
    A3344_cnt = cnt;
    free(mask);
}

seq_t a(idx_t n) {
    if (n > A3344_cnt) {
        long long L = (long long)n * 16 + 1000;
        for (;;) {
            A3344_build(L);
            if (A3344_cnt >= n) break;
            L *= 2;
        }
    }
    return (seq_t)A3344_terms[n - 1];
}
