/*
 * A222949  Numbers that are a sum of four nonzero squares where the summands
 *          have no common square factor > 1.
 * https://oeis.org/A222949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for the nth integer that has a primitive
   representation as a sum of four non‑zero squares.  The function
   iterates through the positive integers starting at 4 and counts
   those for which a primitive representation exists, stopping when the
   count reaches n.  This approach works for all supported build types:
   integer seq_t of any width and arbitrary‑precision seq_t. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* True if m can be expressed as a^2+b^2+c^2+d^2 with
   0 < a <= b <= c <= d and gcd(a,b,c,d)=1.                       */
static bool has_primitive(seq_t m)
{
    if (m < (seq_t)4) return false; /* cannot be sum of four positive squares */

    /* a ranges from 1 to floor(sqrt(m/4))                                 */
    seq_t limitA = floor_sqrt(m / (seq_t)4);
    for (idx_t a = 1; a <= (idx_t)limitA; ++a) {
        seq_t rem1 = m - (seq_t)a * (seq_t)a;

        seq_t limitB = floor_sqrt(rem1 / (seq_t)3);
        for (idx_t b = a; b <= (idx_t)limitB; ++b) {
            seq_t rem2 = rem1 - (seq_t)b * (seq_t)b;

            seq_t limitC = floor_sqrt(rem2 / (seq_t)2);
            for (idx_t c = b; c <= (idx_t)limitC; ++c) {
                seq_t rem3 = rem2 - (seq_t)c * (seq_t)c;
                if (rem3 == (seq_t)0) continue;          /* d would be 0 */

                seq_t d = floor_sqrt(rem3);
                if (d < (seq_t)c) continue;              /* enforce nondecreasing order */
                if (d * d != rem3) continue;              /* not a perfect square */

                seq_t g = gcd((seq_t)a, (seq_t)b);
                g = gcd(g, (seq_t)c);
                g = gcd(g, d);
                if (g == (seq_t)1) return true;          /* primitive representation found */
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1, so 0 is unused */

    idx_t count = 0;          /* number of valid terms found so far */
    idx_t m     = 4;          /* candidate integer to test */

    while (1) {
        if (has_primitive((seq_t)m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        ++m;
    }
}
