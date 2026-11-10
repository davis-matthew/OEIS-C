/*
 * A071338  Duplicate of A078826.
 * https://oeis.org/A071338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = number of distinct primes among the binary substrings of
 *            n (same as A078826)
 * Range:     i8 n <= 35, i16 n <= 35, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static int A071338_cmp(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;
    if (num == 0) return 0;
    unsigned len = 0;
    for (uint64_t t = num; t; t >>= 1) ++len;

    uint64_t vals[2100];
    size_t cnt = 0;
    for (unsigned start = 0; start < len; ++start) {
        uint64_t value = 0;
        for (unsigned end = start; end < len; ++end) {
            value = (value << 1) | ((num >> (len - 1 - end)) & 1ULL);
            if (value >= 2 && is_prime_u64(value)) vals[cnt++] = value;
        }
    }
    if (cnt == 0) return 0;
    qsort(vals, cnt, sizeof(uint64_t), A071338_cmp);
    seq_t res = 1;
    for (size_t i = 1; i < cnt; ++i)
        if (vals[i] != vals[i - 1]) ++res;
    return res;
}
