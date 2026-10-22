/*
 * A004435  Positive integers that are not the sum of 2 distinct square
 *          integers.
 * https://oeis.org/A004435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer that cannot be expressed as
 *            the sum of 2 distinct perfect squares
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t isqrt_seq(seq_t x) {
    if (x == 0) return 0;
    if (x == 1) return 1;
    seq_t rt = x;
    seq_t next = (rt + x / rt) / 2;
    while (next < rt) {
        rt = next;
        next = (rt + x / rt) / 2;
    }
    return rt;
}

static bool is_sum_of_two_distinct_squares(seq_t m) {
    seq_t a = 0;
    while (true) {
        seq_t a_sq = a * a;
        if (a_sq >= m) break;
        seq_t b_sq = m - a_sq;
        seq_t b = isqrt_seq(b_sq);
        if (b * b == b_sq && a != b) {
            return true;
        }
        a++;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    while (cache_len < n) {
        seq_t m = (cache_len > 0) ? cache[cache_len - 1] + 1 : 1;
        while (is_sum_of_two_distinct_squares(m)) {
            m++;
        }
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        cache[cache_len] = m;
        cache_len++;
    }

    return cache[n - 1];
}
