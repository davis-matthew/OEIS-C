/*
 * A080161  Indices of Wonderful Demlo numbers A002477 whose digit sums are
 *          squares.
 * https://oeis.org/A080161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the digit sum of A002477(k) = ((10^k-1)/9)^2 is 81*floor(k/9) +
 *            (k mod 9)^2. a(n) is the n-th k >= 1 for which that sum is a
 *            square. The code finds these k by searching over squares t^2 =
 *            81*m + r^2 (0 <= r <= 8) and setting k = 9*m + r
 * Range:     u8 n <= 17, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9199 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A080161_terms = 0;
static uint64_t A080161_complete = 0;

static int A080161_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A080161_build(uint64_t need)
{
    uint64_t T = 64;
    for (;;) {
        uint64_t t, cnt = 0, bound, i, good = 0;
        uint64_t *buf = (uint64_t *)malloc((size_t)(T + 1) * sizeof(uint64_t));
        for (t = 1; t <= T; t++) {
            uint64_t s = t * t, r;
            for (r = 0; r <= 8; r++) {
                uint64_t rr = r * r;
                if (s >= rr && (s - rr) % 81 == 0) {
                    uint64_t k = (s - rr) / 81 * 9 + r;
                    if (k >= 1) buf[cnt++] = k;
                    break; /* r^2 mod 81 are distinct, so at most one r */
                }
            }
        }
        qsort(buf, (size_t)cnt, sizeof(uint64_t), A080161_cmp);
        /* any k with digit sum s > T^2 satisfies k >= (s-64)/9 > (T^2-64)/9 */
        bound = (T * T - 64) / 9;
        for (i = 0; i < cnt; i++)
            if (buf[i] <= bound) good++;
        if (good >= need) {
            free(A080161_terms);
            A080161_terms = buf;
            A080161_complete = good;
            return;
        }
        free(buf);
        T *= 2;
    }
}

seq_t a(idx_t n)
{
    if ((uint64_t)n > A080161_complete)
        A080161_build((uint64_t)n);
    return (seq_t)A080161_terms[n - 1];
}
