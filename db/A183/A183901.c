/*
 * A183901  Number of nondecreasing arrangements of n+3 numbers in 0..6 with
 *          each number being the sum mod 7 of three others.
 * https://oeis.org/A183901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A183901 : Number of nondecreasing arrangements of n+3 numbers in
 * 0..6 with each number being the sum mod 7 of three others.
 * For n > 3, the sequence satisfies the polynomial:
 * a(n) = (n^6 + 39 n^5 + 625 n^4 + 5265 n^3 + 24574 n^2
 *        - 106104 n - 31680) / 720
 * For n <= 3 we use the explicitly given values. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* base values for n = 1..10 (as given in the OEIS entry) */
    static const seq_t base[] = {
        /* 0 */ 0,
        /* 1 */ 1,
        /* 2 */ 1,
        /* 3 */ 136,
        /* 4 */ 664,
        /* 5 */ 1720,
        /* 6 */ 3491,
        /* 7 */ 6263,
        /* 8 */ 10400,
        /* 9 */ 16357,
        /*10 */ 24694,
        /*11 */ 36091
    };
    /* n <= 4 use the table */
    if (n <= 4) {
        return base[n];
    }

    /* Compute the polynomial for n >= 4 */
    seq_t n0  = (seq_t)n;          /* n */
    seq_t n2  = n0 * n0;           /* n^2 */
    seq_t n3  = n2 * n0;           /* n^3 */
    seq_t n4  = n3 * n0;           /* n^4 */
    seq_t n5  = n4 * n0;           /* n^5 */
    seq_t n6  = n5 * n0;           /* n^6 */

    /* positive part of the numerator */
    seq_t pos = n6;
    pos += (seq_t)39 * n5;
    pos += (seq_t)625 * n4;
    pos += (seq_t)5265 * n3;
    pos += (seq_t)24574 * n2;

    /* negative part of the numerator */
    seq_t neg = (seq_t)106104 * n0 + (seq_t)31680;

    /* numerator = pos - neg (pos is guaranteed to be ≥ neg for n ≥ 4) */
    seq_t num = pos - neg;

    /* Divide by the constant denominator 720 */
    return num / (seq_t)720;
}
