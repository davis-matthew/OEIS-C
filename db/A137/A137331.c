/*
 * A137331  a(n) = 1 if the binary weight of n is prime, otherwise 0.
 * https://oeis.org/A137331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 if the binary weight of n is prime, otherwise 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 0‑based index type */
extern seq_t a(idx_t n);

static const uint8_t _is_prime_weight[65] = {
    0, /* 0 */
    0, /* 1 */
    1, /* 2 */
    1, /* 3 */
    0, /* 4 */
    1, /* 5 */
    0, /* 6 */
    1, /* 7 */
    0, /* 8 */
    0, /* 9 */
    0, /*10 */
    1, /*11 */
    0, /*12 */
    1, /*13 */
    0, /*14 */
    0, /*15 */
    0, /*16 */
    1, /*17 */
    0, /*18 */
    1, /*19 */
    0, /*20 */
    0, /*21 */
    0, /*22 */
    1, /*23 */
    0, /*24 */
    0, /*25 */
    0, /*26 */
    0, /*27 */
    0, /*28 */
    1, /*29 */
    0, /*30 */
    1, /*31 */
    0, /*32 */
    0, /*33 */
    0, /*34 */
    0, /*35 */
    0, /*36 */
    1, /*37 */
    0, /*38 */
    0, /*39 */
    0, /*40 */
    1, /*41 */
    0, /*42 */
    1, /*43 */
    0, /*44 */
    0, /*45 */
    0, /*46 */
    1, /*47 */
    0, /*48 */
    0, /*49 */
    0, /*50 */
    0, /*51 */
    0, /*52 */
    1, /*53 */
    0, /*54 */
    0, /*55 */
    0, /*56 */
    0, /*57 */
    0, /*58 */
    1, /*59 */
    0, /*60 */
    1, /*61 */
    0, /*62 */
    0, /*63 */
    0  /*64 */
};

seq_t a(idx_t n)
{
    /* Compute the binary weight of n */
    idx_t x = n;
    uint8_t pop = 0;
    while (x) {
        pop += (uint8_t)(x & 1);
        x >>= 1;
    }
    return _is_prime_weight[pop] ? (seq_t)1 : (seq_t)0;
}
