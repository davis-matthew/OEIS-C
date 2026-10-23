/*
 * A168037  Period length 18: repeat 0,1,2,0,8,7,0,4,5,0,5,4,0,7,8,0,2,1.
 * https://oeis.org/A168037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A129194(n) mod 9
 * Range:     u8 n <= 0, u16 none, u32 n <= 0, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute the nth term of OEIS sequence A168037.
   The sequence is periodic with period 18:
   a(n) = a(n-18).
   The values are the decimal digits of the rational
   447668335336223 / 37037037037037037, taken modulo 9.
   The sequence is generated once on first call. */

seq_t a(idx_t n) {
    /* 18-periodic table of digits (computed lazily). */
    static seq_t table[18];
    static bool init = false;
    if (!init) {
        init = true;
        const seq_t NUM = (seq_t)447668335336223ULL;   /* numerator */
        const seq_t DEN = (seq_t)37037037037037037ULL; /* denominator */
        seq_t r = NUM;
        for (idx_t i = 0; i < 18; ++i) {
            r *= (seq_t)10;
            table[i] = r / DEN;
            r %= DEN;
        }
    }
    return table[n % 18];
}
