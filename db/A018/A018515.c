/*
 * A018515  Divisors of 552.
 * https://oeis.org/A018515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 552
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compare two seq_t values for qsort */
static int compare_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Precompute the sorted divisors of 552 */
static void init_divisors(seq_t *divisors, size_t *count)
{
    /* 552 = 2^3 * 3^1 * 23^1, so exponents: e2 ∈ [0,3], e3 ∈ [0,1], e23 ∈ [0,1] */
    seq_t temp[16];
    size_t idx = 0;
    for (int e2 = 0; e2 <= 3; ++e2) {
        seq_t v2 = (seq_t)1 << e2;          /* 2^e2 */
        for (int e3 = 0; e3 <= 1; ++e3) {
            seq_t v3 = e3 ? (seq_t)3 : (seq_t)1;   /* 3^e3 */
            for (int e23 = 0; e23 <= 1; ++e23) {
                seq_t v23 = e23 ? (seq_t)23 : (seq_t)1; /* 23^e23 */
                temp[idx++] = v2 * v3 * v23;
            }
        }
    }
    /* idx should be 16 */
    qsort(temp, idx, sizeof(seq_t), compare_seq);
    for (size_t i = 0; i < idx; ++i) {
        divisors[i] = temp[i];
    }
    *count = idx;
}

seq_t a(idx_t n)
{
    /* Static cache of divisors */
    static seq_t cache[16];
    static size_t cached_count = 0;
    if (cached_count == 0) {
        init_divisors(cache, &cached_count);
    }

    if (n == 0) {                 /* offset starts at 1, safe guard */
        return (seq_t)0;
    }
    /* n-1 is safe because n >= 1 */
    idx_t idx = n - 1;
    if (idx < cached_count) {
        return cache[idx];
    }
    /* For n beyond the finite sequence, return 0 (undefined) */
    return (seq_t)0;
}
