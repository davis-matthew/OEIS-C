/*
 * A216475  The number of numbers coprime to and less than n+2, excluding 2.
 * https://oeis.org/A216475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n+2) - (n & 1)
 * Range:     u8 n <= 259, u16 n <= 10005, u32 n <= 10005, u64 n <= 10005,
 *            u128 n <= 10005, big any n
 * Validated: standard, pass, 86 terms through n = 8650 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Euler’s totient: a(2n) = φ(2n+2), a(2n+1) = φ(2n+3)-1) */
#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n) {
    idx_t m = n + 2;         /* compute n+2 */
    seq_t val = totient(m);      /* φ(n+2) */
    if (n & 1)               /* subtract 1 if n is odd */
        val -= 1;
    return val;
}
