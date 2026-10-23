/*
 * A178936  a(n) = floor((2*3^n+3*2^n)/5).
 * https://oeis.org/A178936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*3^n + 3*2^n)/5)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Computes A178936: a(n) = ⌊(2·3^n + 3·2^n)/5⌋  */
seq_t a(idx_t n)
{
    /* Work with 128‑bit integers when the result fits in <=64‑bit types   */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 p3 = 1;
        unsigned __int128 p2 = 1;
        for (idx_t i = 0; i < n; ++i) {
            p3 *= 3;
            p2 *= 2;
        }
        unsigned __int128 num = 2 * p3 + 3 * p2;
        return (seq_t)(num / 5);
    }

    /* For arbitrary‑precision seq_t use the generic big_pow routine */
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);
    seq_t num = p3 * (seq_t)2 + p2 * (seq_t)3;
    return num / (seq_t)5;
}
