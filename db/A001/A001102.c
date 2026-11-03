/*
 * A001102  Numbers k such that k / (sum of digits of k) is a square.
 * https://oeis.org/A001102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   search. Every term has the form k = s*m^2 with s = digitsum(k)
 *            <= 9*(number of digits), so for each digit length D and each s
 *            <= 9D, try every k = s*m^2 with D digits and keep it when
 *            digitsum(k) == s. Then sort each decade's terms
 * Range:     u8 n <= 20, u16 n <= 237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9688 (2026-11-01)
 * Helpers:   digit_sum_u64_to_u32, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum_u64_to_u32.h"
#include "../utils/isqrt_u64.h"

static int a1102_cmp(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static uint64_t *a1102_terms = NULL;
static size_t a1102_cnt = 0, a1102_cap = 0;
static int a1102_done_digits = 0;

static void a1102_push(uint64_t v) {
    if (a1102_cnt == a1102_cap) {
        a1102_cap = a1102_cap ? a1102_cap * 2 : 1024;
        a1102_terms = (uint64_t *)realloc(a1102_terms, a1102_cap * sizeof(uint64_t));
    }
    a1102_terms[a1102_cnt++] = v;
}

static void a1102_decade(int D) {
    uint64_t lo = 1, hi;
    int i;
    for (i = 1; i < D; i++) lo *= 10;
    hi = (D == 20) ? UINT64_MAX : lo * 10; /* k in [lo, hi) */
    size_t start = a1102_cnt;
    unsigned s;
    for (s = 1; s <= (unsigned)(9 * D); s++) {
        uint64_t m = isqrt_u64((lo - 1) / s);
        while (m > 0 && s * m * m >= lo) m--;
        while (s * m * m < lo) m++;
        for (;;) {
            uint64_t m2 = m * m;
            if (m2 > (hi - 1) / s) break;
            uint64_t k = s * m2;
            if (k >= hi) break;
            if (digit_sum_u64_to_u32(k) == s) a1102_push(k);
            m++;
        }
    }
    qsort(a1102_terms + start, a1102_cnt - start, sizeof(uint64_t), a1102_cmp);
}

seq_t a(idx_t n) {
    while ((idx_t)a1102_cnt < n && a1102_done_digits < 19) {
        a1102_done_digits++;
        a1102_decade(a1102_done_digits);
    }
    if ((idx_t)a1102_cnt < n) return 0;
    return (seq_t)a1102_terms[n - 1];
}
