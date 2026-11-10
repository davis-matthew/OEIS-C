/*
 * A077578  Terms of A062273 which are multiples of their indices.
 * https://oeis.org/A077578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A062273(A077579(n)), where A077579 lists the m such that
 *            m divides A062273(m); A062273(m) = Sum_{i=1..m} ((m*(m-1)/2 + i)
 *            mod 10) * 10^(m-i). Found by a linear search over m
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A062273_a(idx_t n)
{
    seq_t base = (seq_t)(((uint64_t)n * ((uint64_t)n - 1) / 2) % 10U);
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = (base + (seq_t)i) % 10U;
        result = result * 10U + digit;
    }
    return result;
}

static bool divides_term(uint64_t m)
{
    uint64_t base = (m * (m - 1) / 2) % 10U;
    uint64_t r = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        uint64_t d = (base + i) % 10U;
        r = (r * 10U + d) % m;
    }
    return r == 0;
}

#define A077579_CAP 512
static uint64_t A077579_found[A077579_CAP];
static idx_t A077579_cnt = 0;
static uint64_t A077579_next = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n > A077579_CAP) {
        /* uncached fallback */
        idx_t c = 0;
        for (uint64_t m = 1;; ++m) {
            if (divides_term(m)) {
                if (++c == n)
                    return A062273_a((idx_t)m);
            }
        }
    }
    while (A077579_cnt < n) {
        uint64_t m = A077579_next++;
        if (divides_term(m))
            A077579_found[A077579_cnt++] = m;
    }
    return A062273_a((idx_t)A077579_found[n - 1]);
}
