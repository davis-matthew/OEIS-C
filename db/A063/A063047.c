/*
 * A063047  Minimum m where (c_n)^m is mutinous (i.e., part of sequence
 *          A027854), where c_n is the n-th positive integer not a prime
 *          power.
 * https://oeis.org/A063047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 1 with r^m > p, where c_n is the n-th
 *            non-prime-power, p its largest prime factor, p^k || c_n, r =
 *            c_n/p^k (search)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A063047_vals = NULL;
static size_t A063047_cnt = 0, A063047_cap = 0;
static uint64_t A063047_next = 6;

/* returns 0 if x is a prime power, otherwise the answer m */
static uint32_t A063047_eval(uint64_t x)
{
    uint64_t t = x, p = 0, pk = 1;
    int distinct = 0;
    for (uint64_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            uint64_t q = 1;
            while (t % d == 0) { t /= d; q *= d; }
            p = d; pk = q; distinct++;
        }
    }
    if (t > 1) { p = t; pk = t; distinct++; }
    if (distinct < 2) return 0;
    uint64_t r = x / pk;
    uint64_t v = r;
    uint32_t m = 1;
    while (v <= p) { v *= r; m++; }
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A063047_cnt < (size_t)n) {
        uint64_t x = A063047_next++;
        uint32_t m = A063047_eval(x);
        if (!m) continue;
        if (A063047_cnt == A063047_cap) {
            size_t nc = A063047_cap ? A063047_cap * 2 : 1024;
            uint32_t *tmp = (uint32_t *)realloc(A063047_vals, nc * sizeof(uint32_t));
            if (!tmp) return (seq_t)0;
            A063047_vals = tmp;
            A063047_cap = nc;
        }
        A063047_vals[A063047_cnt++] = m;
    }
    return (seq_t)A063047_vals[n - 1];
}
