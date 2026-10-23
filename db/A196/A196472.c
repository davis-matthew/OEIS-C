/*
 * A196472  a(1)=1; a(n) = floor((3 + sqrt(21))*a(n-1)/2) for n > 1.
 * https://oeis.org/A196472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 31,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196472:
   a(1) = 1
   a(n) = floor((3 + sqrt(21)) * a(n-1) / 2)   for n > 1
   (OEIS: contains only odd numbers)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Integer square‑root for unsigned __int128 (returns floor(sqrt(x))) */
static inline unsigned __int128 u128_sqrt(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid == 0) { hi = 0; break; }
        if (mid <= x / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* Compute a(n) for any seq_t (built‑in or arbitrary‑precision) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, but not expected */

    seq_t prev = 1;                  /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i) {
        if (sizeof(seq_t) <= 8) {    /* built‑in integer path (uint8_t..uint64_t) */
            unsigned __int128 p = (unsigned __int128)prev;
            unsigned __int128 prod = p * p * 21;
            unsigned __int128 sqrtp = u128_sqrt(prod);
            unsigned __int128 next = ((unsigned __int128)3 * p + sqrtp) >> 1;
            prev = (seq_t)next;     /* truncation occurs only for very large n */
        } else {                    /* arbitrary‑precision integer path */
            seq_t prod = prev;
            prod *= prod;
            prod *= 21;
            seq_t sqrtp = floor_sqrt(prod);
            seq_t next = (prev * 3 + sqrtp) / 2;
            prev = next;
        }
    }
    return prev;
}
