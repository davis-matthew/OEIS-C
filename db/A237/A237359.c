/*
 * A237359  a(n) is the largest integer that makes A237354(a(n))=n.
 * https://oeis.org/A237359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = largest m with A237354(m) = n, where A237354(m) = max
 *            over g+h=m of omega(g)+omega(h). Computed by a downward search
 *            from a heuristic bound (8*a(n-1)+64), using a sieve of omega
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t A237359_memo[64];
static bool A237359_have[64];

static bool A237359_exists(idx_t m, unsigned t, const unsigned char *om,
                           const uint32_t *list, size_t cnt)
{
    for (size_t i = 0; i < cnt; ++i) {
        idx_t x = list[i];
        if (x >= m) break;
        if ((unsigned)om[x] + (unsigned)om[m - x] >= t) return true;
    }
    return false;
}

static idx_t A237359_compute(idx_t n, idx_t prev)
{
    idx_t L = 8 * prev + 64;
    unsigned char *om = (unsigned char *)malloc((size_t)L + 1);
    if (!om) return 0;
    for (size_t i = 0; i <= (size_t)L; ++i) om[i] = 0;
    for (size_t p = 2; p <= (size_t)L; ++p) {
        if (om[p] == 0) {
            for (size_t k = p; k <= (size_t)L; k += p) om[k]++;
        }
    }
    unsigned c = (unsigned)((n + 1) / 2);
    size_t cnt = 0;
    for (size_t i = 1; i <= (size_t)L; ++i) if (om[i] >= c) cnt++;
    uint32_t *list = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    if (!list) { free(om); return 0; }
    size_t k = 0;
    for (size_t i = 1; i <= (size_t)L; ++i) if (om[i] >= c) list[k++] = (uint32_t)i;

    idx_t res = 0;
    for (idx_t m = L; m >= 2; --m) {
        if (A237359_exists(m, (unsigned)n + 1, om, list, cnt)) continue;
        if (A237359_exists(m, (unsigned)n, om, list, cnt)) { res = m; break; }
    }
    free(list);
    free(om);
    return res;
}

seq_t a(idx_t n)
{
    if (n >= 64) return 0;
    idx_t prev = 0;
    for (idx_t j = 0; j <= n; ++j) {
        if (!A237359_have[j]) {
            A237359_memo[j] = A237359_compute(j, prev);
            A237359_have[j] = true;
        }
        prev = A237359_memo[j];
    }
    return (seq_t)A237359_memo[n];
}
