/*
 * A214791  Numbers n such that n+8 = A214789(n).
 * https://oeis.org/A214791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th k such that A214789(k) = k+8, i.e. the
 *            least m>=1 with F(m) == -8 (mod k) is m = k+7 (Fibonacci mod k)
 * Range:     u8 n <= 15, u16 n <= 1285, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 55 terms through n = 3851 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A214791_u128;

static uint64_t A214791_mm(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((A214791_u128)x * y) % m);
}

/* F(k) mod m via fast doubling, m >= 2 */
static uint64_t A214791_fibmod(uint64_t k, uint64_t m)
{
    uint64_t a = 0, b = 1 % m;
    int i;
    for (i = 63; i >= 0; --i) {
        uint64_t t = (2 * (b % m) % m + m - a) % m;
        uint64_t c = A214791_mm(a, t, m);
        uint64_t d = (A214791_mm(a, a, m) + A214791_mm(b, b, m)) % m;
        if ((k >> i) & 1) {
            a = d;
            b = (c + d) % m;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

static bool A214791_test(uint64_t n)
{
    uint64_t m, fp, fc, r, j;
    if (n < 2) return false;
    if ((A214791_fibmod(n + 7, n) + 8) % n != 0) return false;
    /* verify no smaller m in 1..n+6 */
    fp = 0; fc = 1 % n;
    for (j = 1; j < n + 7; ++j) {
        if ((fc + 8) % n == 0) return false;
        r = fp + fc;
        if (r >= n) r -= n;
        fp = fc;
        fc = r;
    }
    (void)m;
    return true;
}

static idx_t *A214791_terms = 0;
static size_t A214791_cnt = 0, A214791_cap = 0;
static idx_t A214791_next = 2;

seq_t a(idx_t n)
{
    while (A214791_cnt < (size_t)n) {
        if (A214791_test(A214791_next)) {
            if (A214791_cnt == A214791_cap) {
                A214791_cap = A214791_cap ? A214791_cap * 2 : 256;
                A214791_terms = (idx_t *)realloc(A214791_terms, A214791_cap * sizeof(idx_t));
            }
            A214791_terms[A214791_cnt++] = A214791_next;
        }
        ++A214791_next;
    }
    return (seq_t)A214791_terms[n - 1];
}
