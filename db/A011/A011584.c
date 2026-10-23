/*
 * A011584  Legendre symbol (n,17).
 * https://oeis.org/A011584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Legendre symbol (n,17): a(0) = 0, for n > 0: if gcd(n,17)
 *            > 1 => a(n) = 0, else if n is a quadratic residue mod 17 => a(n)
 *            = 1, else a(n) = -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t table[17] = {
        (seq_t)0,  /* 0 mod 17 */
        (seq_t)1,  /* 1 */
        (seq_t)1,  /* 2 */
        (seq_t)-1, /* 3 */
        (seq_t)1,  /* 4 */
        (seq_t)-1, /* 5 */
        (seq_t)-1, /* 6 */
        (seq_t)-1, /* 7 */
        (seq_t)1,  /* 8 */
        (seq_t)1,  /* 9 */
        (seq_t)-1, /*10 */
        (seq_t)-1, /*11 */
        (seq_t)-1, /*12 */
        (seq_t)1,  /*13 */
        (seq_t)-1, /*14 */
        (seq_t)1,  /*15 */
        (seq_t)1   /*16 */
    };
    return table[n % 17];
}
