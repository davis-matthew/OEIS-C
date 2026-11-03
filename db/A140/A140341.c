/*
 * A140341  The number of bits needed to write the universal code for an Elias
 *          delta coding, the simplest asymptotically optimal code.
 * https://oeis.org/A140341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor(log2( floor(log2(n)) + 1 )) + floor(log2(n)) +
 *            1 - 2
 * Range:     u8 n <= 489, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9541 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   n >= 1, uses standard Elias delta coding bit-length */

/* Utilities */
#include <stdint.h>
#include "../utils/bit_length.h"

/* Computes the number of bits needed to write the Elias delta code for the integer n. */
seq_t a(idx_t n)
{
    /* n >= 1 according to the sequence definition */
    seq_t n_val = (seq_t)n;                    /* promote index to seq_t */
    unsigned log2n = bit_length(n_val) - 1;   /* floor(log2(n))          */
    seq_t b = (seq_t)(log2n + 1);              /* length of binary of n   */
    unsigned log2b = bit_length(b) - 1;       /* floor(log2(b))          */
    seq_t u = (seq_t)(log2b + 1);              /* length of binary of b   */
    seq_t res = 2 * u + b - 2;                 /* a(n) as derived        */
    return res;
}
