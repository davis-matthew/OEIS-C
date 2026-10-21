/*
 * A071693  Number of ways to write n as n = x*y*z + x + y + z where 0 <= x <=
 *          y <= z <= n.
 * https://oeis.org/A071693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) counts integer triples (x, y, z) with 0 ≤ x ≤ y ≤ z ≤ n
 * satisfying n = x*y*z + x + y + z.
 * The implementation iterates over x ≥ 1 and y ≥ x, solves for
 * z = (n - x - y)/(x*y + 1), and counts solutions with y ≤ z.
 * All solutions with x = 0 are counted directly as floor(n/2)+1.
 */
 
#include <stdint.h>
 
seq_t a(idx_t n) {
    /* base count coming from all solutions with x = 0:
       n = y + z, 0 ≤ y ≤ z ≤ n  =>  y can be 0 … ⌊n/2⌋  (inclusive) */
    seq_t count = (seq_t)(n / 2) + 1;
    if (n == 0) return count;   /* f(0) = 1, no further loops needed */
 
    seq_t ns = (seq_t)n;
 
    for (idx_t x = 1; x <= n; ++x) {
        /* y ranges from x up to n - x, because we need n - x - y >= 0 */
        for (idx_t y = x; y <= n - x; ++y) {
            idx_t numI = n - x - y;          /* n - x - y  (no negative here) */
            if (numI == 0) break;            /* z would be 0, impossible for y > 0 */
 
            unsigned __int128 prod = (unsigned __int128)x * y;
            unsigned __int128 denom128 = prod + 1;
            unsigned __int128 num128   = numI;
 
            if (denom128 > num128) break;    /* further y will only increase denom and decrease num */
 
            seq_t denom = (seq_t)denom128;    /* denom <= num <= n, safe cast */
            seq_t num   = (seq_t)numI;
 
            if ((num % denom) == 0) {
                seq_t z = num / denom;
                if ((seq_t)y <= z && z <= ns) {
                    ++count;
                }
            }
        }
    }
    return count;
}
