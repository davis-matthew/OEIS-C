/*
 * A082664  Numbers k such that A082647(k) = A000005(k) - 1.
 * https://oeis.org/A082664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k such that A082647(k) = A000005(k) - 1, where
 *            A082647(k) = number of odd divisors d of k with d <=
 *            floor(sqrt(2k)); found by sequential search with cached terms
 * Range:     u8 n <= 68, u16 n <= 7388, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9885 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t A082664_isqrt(uint64_t x)
{
    uint64_t r = 0, b = (uint64_t)1 << 62;
    while (b > x) b >>= 2;
    while (b) {
        if (x >= r + b) { x -= r + b; r = (r >> 1) + b; }
        else r >>= 1;
        b >>= 2;
    }
    return r;
}

static int A082664_test(uint64_t k)
{
    uint64_t lim = A082664_isqrt(2 * k);
    uint64_t odd = 0, tau = 0, d;
    for (d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            tau += (d * d == k) ? 1 : 2;
        }
    }
    for (d = 1; d <= lim; d += 2) {
        if (k % d == 0) odd++;
    }
    return odd == tau - 1;
}

static uint64_t *A082664_tbl = NULL;
static size_t A082664_cnt = 0, A082664_cap = 0;
static uint64_t A082664_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A082664_cnt < (size_t)n) {
        if (A082664_test(A082664_next)) {
            if (A082664_cnt == A082664_cap) {
                size_t nc = A082664_cap ? A082664_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A082664_tbl, nc * sizeof(uint64_t));
                if (!p) return 0;
                A082664_tbl = p;
                A082664_cap = nc;
            }
            A082664_tbl[A082664_cnt++] = A082664_next;
        }
        A082664_next++;
    }
    return (seq_t)A082664_tbl[(size_t)n - 1];
}
