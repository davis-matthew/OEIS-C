/*
 * A225913  Composite numbers coprime to 6 such that A179382(n) =
 *          A000265(n-1), the odd part of n-1.
 * https://oeis.org/A225913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th composite m with gcd(m,6)=1 such that
 *            the cycle length of x -> A000265(x + 2m - 1), starting at x = 1,
 *            equals A000265(m-1)
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A225913_composite(uint64_t m)
{
    uint64_t d;
    for (d = 5; d * d <= m; d += 2)
        if (m % d == 0)
            return true;
    return false;
}

static bool A225913_check(uint64_t m)
{
    uint64_t M = 2 * m - 1;
    uint64_t target = m - 1;
    while ((target & 1) == 0)
        target >>= 1;
    uint64_t cur = 1, len = 0;
    do
    {
        uint64_t t = cur + M;
        while ((t & 1) == 0)
            t >>= 1;
        cur = t;
        ++len;
        if (len > target)
            return false;
    } while (cur != 1);
    return len == target;
}

#define A225913_MAX 4096
static uint64_t A225913_terms[A225913_MAX];
static uint64_t A225913_count = 0;
static uint64_t A225913_next = 5;

seq_t a(idx_t n)
{
    if (n <= A225913_count)
        return (seq_t)A225913_terms[n - 1];
    uint64_t m = A225913_next;
    for (;; m += 2)
    {
        if (m % 3 == 0)
            continue;
        if (!A225913_composite(m))
            continue;
        if (A225913_check(m))
        {
            if (A225913_count < A225913_MAX)
                A225913_terms[A225913_count] = m;
            ++A225913_count;
            if (A225913_count == (uint64_t)n)
            {
                A225913_next = m + 2;
                return (seq_t)m;
            }
        }
    }
}
