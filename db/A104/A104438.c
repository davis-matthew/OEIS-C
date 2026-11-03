/*
 * A104438  Number of ways to split 1, 2, 3, ..., 5n into 5 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20 for n ≥ 6; for n ≤ 5 the values are obtained by an
 *            exhaustive back‑tracking search that enumerates all partitions
 *            of {1,…,5n} into five arithmetic progressions each of length n
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t dfs(uint32_t usedMask, idx_t n, uint32_t total, uint32_t maxD)
{
    if (usedMask == ((uint32_t)1u << total) - 1u)   /* all numbers used */
        return 1;

    /* smallest unused number (0‑based for the bit mask) */
    uint32_t s = 0;
    while (usedMask & (1u << s)) s++;
    uint32_t startVal = s + 1;                    /* value in the set */

    /* maximum difference allowed for this start */
    uint32_t maxDStart = (total - startVal) / (n - 1);

    seq_t count = 0;
    for (uint32_t d = 1; d <= maxDStart; ++d) {
        uint32_t mask = 0;
        bool ok = true;
        for (uint32_t k = 0; k < (uint32_t)n; ++k) {
            uint32_t val = startVal + k * d;      /* 1‑based value */
            if (val > total) { ok = false; break; }
            uint32_t bit = 1u << (val - 1);
            if (usedMask & bit) { ok = false; break; }
            mask |= bit;
        }
        if (!ok) continue;
        count += dfs(usedMask | mask, n, total, maxD);
    }
    return count;
}

static seq_t count_partitions(idx_t n)
{
    if (n == 1) return 1;                 /* trivial case */
    uint32_t total = 5 * (uint32_t)n;     /* total numbers */
    uint32_t maxD = (5 * n - 1) / (n - 1);
    return dfs(0u, n, total, maxD);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n < 1 */
    if (n >= 6) return 20;                /* known to be constant */
    return count_partitions(n);
}
