/*
 * A122458  "Dropping time" of the reduced Collatz iteration starting with
 *          2n+1.
 * https://oeis.org/A122458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that fr^[k](n) < 2n+1, where fr(x) =
 *            (3*x+1)/2^{v2(3*x+1)} and v2 is the 2‑adic valuation. a(0) := 0
 * Range:     u8 n <= 12, u16 n <= 5078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Nonn and reduce Collatz iteration starting at 2n+1. */
    if (n == 0) return (seq_t)0;

    seq_t start = (seq_t)n * 2 + 1;   /* 2n+1, always odd */
    seq_t x     = start;
    seq_t count = 0;

    while (x >= start)
    {
        seq_t t = 3 * x + 1;          /* 3x + 1 */

        /* divide by powers of two until odd: v2(t) */
        while ((t & 1) == 0) t >>= 1;

        x = t;
        count += 1;
    }
    return count;
}
