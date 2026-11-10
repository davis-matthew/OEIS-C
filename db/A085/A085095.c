/*
 * A085095  Index of first occurrence of n in A085094.
 * https://oeis.org/A085095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m such that A085094(m) = n, i.e. the smallest m
 *            for which n*m-1 is a palindrome and k*m-1 is not a palindrome
 *            for any 1 <= k < n (found by search over m)
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* decimal palindrome test on a native 64-bit value */
static bool A085095_is_pal_u64(uint64_t x)
{
    uint64_t r = 0, t = x;
    while (t > 0) {
        r = r * 10 + t % 10;
        t /= 10;
    }
    return r == x;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t m = 1;; ++m) {
        if (!A085095_is_pal_u64(nn * m - 1))
            continue;
        bool ok = true;
        for (uint64_t k = 1; k < nn; ++k) {
            if (A085095_is_pal_u64(k * m - 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            seq_t res = 0;
            /* build result in seq_t without assuming a native width */
            uint64_t mm = m;
            seq_t place = 1;
            while (mm > 0) {
                res += place * (seq_t)(unsigned)(mm % 10);
                mm /= 10;
                if (mm > 0)
                    place *= 10;
            }
            return res;
        }
    }
}
