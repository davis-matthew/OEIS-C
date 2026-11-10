/*
 * A090501  Group the natural numbers as in A090499; sequence gives the number
 *          of members in each group.
 * https://oeis.org/A090501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A090499(n+1)-A090499(n): size of the n-th group; groups
 *            built greedily so each group's divisor-count sum strictly
 *            exceeds the previous group's
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t A090501_dcount(uint64_t x) {
    uint64_t res = 1, p;
    for (p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            res *= (e + 1);
        }
    }
    if (x > 1) res *= 2;
    return res;
}

seq_t a(idx_t n) {
    static uint64_t *sizes = NULL;
    static size_t cap = 0, cnt = 0;
    static uint64_t next_num = 1, prev_sum = 0;

    if (n < 1) return 0;
    while (cnt < (size_t)n) {
        uint64_t sum = 0, len = 0;
        while (sum <= prev_sum) {
            sum += A090501_dcount(next_num);
            ++next_num;
            ++len;
        }
        if (cnt == cap) {
            size_t nc = cap ? cap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(sizes, nc * sizeof(uint64_t));
            if (!t) return 0;
            sizes = t;
            cap = nc;
        }
        sizes[cnt++] = len;
        prev_sum = sum;
    }
    return (seq_t)sizes[(size_t)n - 1];
}
