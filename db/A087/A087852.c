/*
 * A087852  Erroneous generalization of A087851.
 * https://oeis.org/A087852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor((n + sqrt(5*n*n)) / 2) for n ≥ 8; initial values
 *            for n = 1 … 7 are a(1)=1, a(2)=1, a(3)=3, a(4)=3, a(5)=5,
 *            a(6)=4, a(7)=8
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned __int128 value  (floor). */
static inline unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;   /* highest power of 4 ≤ x        */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* explicit values for n = 1 … 7 */
    switch (n) {
    case 1: return (seq_t)1;
    case 2: return (seq_t)1;
    case 3: return (seq_t)3;
    case 4: return (seq_t)3;
    case 5: return (seq_t)5;
    case 6: return (seq_t)4;
    case 7: return (seq_t)8;
    default:
        /* compute floor((n + sqrt(5*n*n)) / 2) */
        unsigned __int128 fn = (unsigned __int128)n;
        unsigned __int128 prod = fn * fn * (unsigned __int128)5;
        unsigned __int128 sqrt_val = isqrt_u128(prod);
        unsigned __int128 sum = fn + sqrt_val;
        return (seq_t)(sum / 2);
    }
}
