/*
 * A118391  Numerator of sum of reciprocals of first n tetrahedral numbers
 *          A000292.
 * https://oeis.org/A118391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = Numerator( 3*n*(n+3)/(2*(n+1)*(n+2)) )
 * Range:     u8 n <= 14, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b) {
    while (b) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /*  n is at least 1. */
    unsigned long long nn = (unsigned long long)n;

    /* Compute numerator and denominator as 128‑bit integers to avoid overflow. */
    unsigned __int128 num = (unsigned __int128)3 * nn * (nn + 3);
    unsigned __int128 den = (unsigned __int128)2 * (nn + 1) * (nn + 2);

    /* Reduce the fraction. */
    unsigned __int128 g = gcd128(num, den);
    unsigned __int128 red_num = num / g;

    /* Convert the result back to seq_t (the caller ensures the value fits). */
    return (seq_t)red_num;
}
