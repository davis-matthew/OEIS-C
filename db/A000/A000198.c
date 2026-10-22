/*
 * A000198  Largest order of automorphism group of a tournament with n nodes.
 * https://oeis.org/A000198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 44, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Helpers:   a198_pow3, a198_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (Alspach & Berggren 1973, Theorem 5):
   a(3r) = 3^r * a(r); a(n) = a(n-1) for n == 1, 2, 4 (mod 9);
   a(9k+7) = a(7)*a(9k); a(9k+8) = max(a(9k+7), a(5)*a(9k+3));
   a(9k+5) = max(a(2)*a(9k+3), a(5)*a(9k), a(7)*a(9k-2)) for k >= 1;
   initial values a(0) = 1, a(1) = 1, a(2) = 1, a(5) = 5, a(7) = 21. */
#include <stdint.h>
#include "../utils/a198_pow3.h"
#include "../utils/a198_rec.h"

seq_t a(idx_t n)
{
    return a198_rec(n);
}
