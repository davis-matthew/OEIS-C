/*
 * A072961  Numbers using only the digits 2 and 5, that are both curved and
 *          straight.
 * https://oeis.org/A072961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1534, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers made of the digits 2 and 5, ordered by increasing numeric
   value.  The n‑th term is obtained by enumerating binary strings of
   increasing length and mapping 0 → 2, 1 → 5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined position */

    /* Find the length of the binary string containing the n-th term. */
    idx_t cumulative = 0;   /* total terms of lengths < len       */
    idx_t power      = 2;   /* 2^1 : number of terms of length 1   */
    idx_t len        = 1;   /* current string length               */

    while (cumulative + power < n) {
        cumulative += power;
        power <<= 1;        /* next power of two (2^(len+1))      */
        len++;
    }

    /* Offset of the term within strings of this length (0‑based). */
    idx_t offset = n - cumulative - 1;

    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned int shift = (unsigned int)(len - 1 - i);
        uint8_t bit = (offset >> shift) & 1;
        result = result * 10 + (bit ? 5 : 2);
    }

    return result;
}
