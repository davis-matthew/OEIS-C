/*
 * A105740  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=7.
 * https://oeis.org/A105740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer a such that * a(n-1)^2 +
 *            4·a(n-2)·a is a perfect square. * initial values: a(1)=1, a(2)=7
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* helper for 128‑bit integer type (extension supported by GNU/Clang) */
typedef unsigned __int128 u128;

/* test whether a 128‑bit integer is a perfect square */
static bool is_square128(u128 x)
{
    u128 lo = 0, hi = ((u128)1 << 64);          /* sqrt(2^128-1) <= 2^64-1 */
    while (lo < hi) {
        u128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    u128 r = lo - 1;
    return r * r == x;
}

/* Cached values up to a maximum needed index (here 10,000). */
static seq_t a_cache[10001];
static bool a_cache_init = false;

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    /* offset: sequence is 1‑based. */
    if (n == 0) return 0;          /* out of range, harmless */
    if (!a_cache_init) {
        /* initialise first two terms */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)7;
        a_cache_init = true;
    }

    /* extend cache up to the needed index */
    for (idx_t i = (idx_t)(a_cache_init ? 3 : 1); i <= n; ++i) {
        /* skip if already computed */
        if (i <= 2) continue;
        seq_t prev1 = a_cache[i - 2]; /* a(i-1) */
        seq_t prev2 = a_cache[i - 3]; /* a(i-2) */

        seq_t cand = 1;
        for (;;) {
            /* compute X = prev1^2 + 4*prev2*cand using 128‑bit */
            u128 X = (u128)prev1 * (u128)prev1
                   + (u128)4 * (u128)prev2 * (u128)cand;
            if (is_square128(X)) {
                a_cache[i - 1] = cand;
                break;
            }
            ++cand;
        }
    }
    return a_cache[n - 1];
}
