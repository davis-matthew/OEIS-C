/*
 * A004014  Norms of vectors in the b.c.c. lattice.
 * https://oeis.org/A004014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve-free scan: m is a term iff m == 3 (mod 8), or m == 4k with
 *            k not of the form 4^i*(8j+7) (k = 0 included)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool in_seq(uint64_t m) {
    if (m % 8 == 3) return true;          /* three odd squares */
    if (m % 4 != 0) return false;
    uint64_t k = m / 4;                   /* three even squares: k is a sum of 3 squares */
    if (k == 0) return true;
    while (k % 4 == 0) k /= 4;
    return k % 8 != 7;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m = 0;
    for (;;) {
        if (in_seq(m)) {
            if (count == n) return (seq_t)m;
            count++;
        }
        m++;
    }
}
