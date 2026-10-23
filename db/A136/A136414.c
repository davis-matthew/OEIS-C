/*
 * A136414  Put the natural numbers together without spaces and read them two
 *          at a time advancing one space each time.
 * https://oeis.org/A136414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = 10*A007376(n) + A007376(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   champerdigit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/champerdigit.h"
/* where A007376(k) is the k‑th digit of the Champernowne constant
 * (the concatenation of natural numbers: 123456789101112...) */

seq_t a(idx_t n) {
    seq_t d1 = champerdigit(n);
    seq_t d2 = champerdigit(n + 1);
    return d1 * 10 + d2;
}
