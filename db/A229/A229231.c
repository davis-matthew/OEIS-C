/*
 * A229231  Arises in complete classification of spherically symmetric static
 *          spacetimes via Noether symmetries.
 * https://oeis.org/A229231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A072966(n+5), 1 <= n <= 6; computed by search: the
 *            numbers m >= 5 that are not a sum of two semiprimes (the
 *            sequence is finite, 6 terms)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A229231_semiprime(unsigned x)
{
    unsigned cnt = 0;
    for (unsigned p = 2; p * p <= x && cnt < 3; ++p) {
        while (x % p == 0) { x /= p; ++cnt; }
    }
    if (x > 1) ++cnt;
    return cnt == 2;
}

static bool A229231_repr(unsigned m)
{
    for (unsigned s = 4; s + 4 <= m; ++s) {
        if (A229231_semiprime(s) && A229231_semiprime(m - s)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static unsigned vals[6];
    static unsigned found = 0;
    if (found == 0) {
        for (unsigned m = 5; m <= 400 && found < 6; ++m) {
            if (!A229231_repr(m)) vals[found++] = m;
        }
        if (found == 0) found = 7; /* never happens; avoid recompute */
    }
    if (n < 1 || n > 6) return 0;
    return (seq_t)vals[n - 1];
}
