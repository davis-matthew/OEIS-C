/*
 * A106913  Primes of the form x^2+xy+14y^2, with x and y nonnegative.
 * https://oeis.org/A106913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the nth prime p that can be written as 
   p = x^2 + x*y + 14*y^2 for some non‑negative integers x,y.
   The function scans successive odd candidates, tests primality
   (using isPrime64) and checks the representation condition.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime64.h"

/* integer square root of a 128‑bit value */
static unsigned long long isqrt128(unsigned __int128 n)
{
    unsigned long long lo = 0, hi = ~0ULL;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* return true if p can be expressed as x^2 + x*y + 14*y^2 */
static bool representable(uint64_t p)
{
    for (uint64_t y = 0;; ++y) {
        unsigned __int128 y2   = (unsigned __int128)y * y;
        unsigned __int128 term = (unsigned __int128)14 * y2;
        if (term > (unsigned __int128)p) break;          /* 4p - 55y^2 < 0  */
        unsigned __int128 D = (unsigned __int128)4 * p - (unsigned __int128)55 * y2;
        if (D < 0) continue;
        unsigned long long s = isqrt128(D);
        if ((unsigned __int128)s * s == D) {
            if (s >= y && ((s - y) & 1ULL) == 0)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    seq_t count = 0;
    uint64_t candidate = 2;

    while (1) {
        if (isPrime64(candidate)) {
            if (representable(candidate)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        /* skip even numbers >2 */
        candidate += (candidate == 2) ? 1 : 2;
    }
}
