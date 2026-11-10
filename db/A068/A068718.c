/*
 * A068718  Boustrophedon transform of A068717 with A068717(1) = -1 instead of
 *          0.
 * https://oeis.org/A068718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |T(n,n)|, T = boustrophedon transform of b(k) =
 *            A068717(k+1) with b(0) = -1; T(n,0) = b(n), T(n,k) = T(n,k-1) +
 *            T(n-1,n-k). Computed as the difference of the transforms of the
 *            +1 and -1 parts of b
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A068718_isqrt(uint64_t v)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid * mid <= v) { ans = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return ans;
}

/* A068717(m) as -1, 0, 1 (with A068717(1) = -1 per this entry). */
static int A068718_sgn(uint64_t m)
{
    if (m == 1) return -1;
    uint64_t root = A068718_isqrt(m);
    if (root * root == m) return 0;
    uint64_t mm = 0, d = 1, a0 = root, ac = root;
    unsigned period = 0;
    for (;;) {
        mm = d * ac - mm;
        d = (m - mm * mm) / d;
        ac = (a0 + mm) / d;
        period++;
        if (ac == 2 * a0) break;
    }
    return (period & 1U) ? -1 : 1;
}

static seq_t A068718_trans(const int *sg, int want, idx_t n)
{
    size_t len = (size_t)n + 2;
    seq_t *prev = mem_new(seq_t, len);
    seq_t *cur = mem_new(seq_t, len);
    for (idx_t r = 0; r <= n; r++) {
        cur[0] = (sg[r] == want) ? 1 : 0;
        for (idx_t k = 1; k <= r; k++)
            cur[k] = cur[k - 1] + prev[r - k];
        seq_t *t = prev; prev = cur; cur = t;
    }
    seq_t res = prev[n];
    mem_free(prev);
    mem_free(cur);
    return res;
}

seq_t a(idx_t n)
{
    int *sg = (int *)malloc(((size_t)n + 1) * sizeof(int));
    for (idx_t k = 0; k <= n; k++) sg[k] = A068718_sgn((uint64_t)k + 1);
    seq_t neg = A068718_trans(sg, -1, n);
    seq_t pos = A068718_trans(sg, 1, n);
    free(sg);
    return neg - pos;
}
