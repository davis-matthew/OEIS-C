/*
 * A124088  10th column of Catalan triangle A009766.
 * https://oeis.org/A124088
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,9) - C(n,7)
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* For n < 17 the behaviour is undefined, but the function
       still returns a non‑negative value from the formula. */
    seq_t c9 = binomial(n, 9);
    seq_t c7 = binomial(n, 7);
    return c9 - c7;
}
