/*
 * A002321  Mertens's function: Sum_{k=1..n} mu(k), where mu is the Moebius
 *          function A008683.
 * https://oeis.org/A002321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, changed
 * Formula:   a(n) = Sum_{k=1..n} mu(k), where mu is the Moebius function
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   moebius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/moebius.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        result += (seq_t)moebius(k);
    }
    return result;
}
