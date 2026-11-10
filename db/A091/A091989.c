/*
 * A091989  Numbers that are binary cyclic right-rotating progressive sum of
 *          digits (see A091821) of exactly 1 natural number.
 * https://oeis.org/A091989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th m such that exactly one k has A091821(k) = m
 *            (search; A091821(k) = (k/len)*popcount(k) + popcount(k >> (len -
 *            k%len)), len = bitlen(k))
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A091989_f(uint64_t n)
{
    unsigned ones = (unsigned)__builtin_popcountll(n);
    unsigned len = 64 - (unsigned)__builtin_clzll(n);
    uint64_t full = n / len;
    uint64_t rem = n % len;
    uint64_t r = full * ones;
    if (rem) r += (unsigned)__builtin_popcountll(n >> (len - rem));
    return r;
}

static int A091989_once(uint64_t m)
{
    uint64_t cnt = 0, k;
    uint64_t hi = (m + 1) * 64;
    for (k = m; k <= hi; k++) {
        if (k == 0) continue;
        if (A091989_f(k) == m) {
            if (++cnt > 1) return 0;
        }
    }
    return cnt == 1;
}

#define A091989_MAX 8192
static uint64_t A091989_terms[A091989_MAX];
static uint64_t A091989_count = 0;
static uint64_t A091989_next = 1;

seq_t a(idx_t n)
{
    if (n == 0 || n > A091989_MAX) return 0;
    while (A091989_count < n) {
        if (A091989_once(A091989_next))
            A091989_terms[A091989_count++] = A091989_next;
        A091989_next++;
    }
    return (seq_t)A091989_terms[n - 1];
}
