/*
 * A025436  Number of partitions of n into 3 distinct squares.
 * https://oeis.org/A025436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of partitions of n into three distinct squares
   (including 0²).  The function enumerates all triples (a,b,c) with
   0 ≤ a < b < c and a²+b²+c² = n.  Complexity is O(√n ²). */
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for non‑negative values of type idx_t (uint64_t). */
static inline idx_t _int_sqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 30;  /* 2³⁰ > sqrt(UINT64_MAX) */
    while (bit > 0) {
        idx_t nxt = r | bit;
        if (nxt * nxt <= x)
            r = nxt;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Count all triples 0 ≤ a < b < c with a²+b²+c² == n. */
    seq_t count = 0;
    idx_t max_a = _int_sqrt(n);
    for (idx_t a = 0; a <= max_a; ++a) {
        idx_t a2 = a * a;
        idx_t rem_ab = n - a2;
        idx_t max_b = _int_sqrt(rem_ab);
        for (idx_t b = a + 1; b <= max_b; ++b) {
            idx_t b2 = b * b;
            idx_t rem = rem_ab - b2;
            if (rem <= 0) continue;                /* c must be > b */
            idx_t c = _int_sqrt(rem);
            if (c > b && c * c == rem) {
                ++count;                           /* (a,b,c) is a valid triple */
            }
        }
    }
    return count;
}
