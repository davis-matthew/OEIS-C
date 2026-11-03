/*
 * A091066  Numbers having in binary representation at least one proper prefix
 *          that is also a suffix.
 * https://oeis.org/A091066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 7703 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091066: Numbers whose binary representation has at least one
   proper prefix that is also a suffix.
   Computed by incremental search from 3 upwards and testing each
   candidate for the existence of a border in its binary string. */

#include <stdbool.h>

static inline bool has_bordered_binary(seq_t x)
{
    /* All odds > 1 are automatically bordered: suffix must end with 1
       and the first bit is always 1. */
    if ((x & 1) != 0)
        return true;

    /* Compute the bit length of x. */
    unsigned bitlen = 0;
    seq_t tmp = x;
    while (tmp) {
        tmp >>= 1;
        bitlen++;
    }
    /* Try all possible border lengths k (1 .. bitlen-1). */
    for (unsigned k = 1; k < bitlen; ++k) {
        seq_t left  = x >> (bitlen - k);          /* top k bits      */
        seq_t mask  = ((seq_t)1 << k) - 1;        /* k low bits mask */
        seq_t right = x & mask;                   /* bottom k bits   */
        if (left == right)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Return n-th element of A091066 (1‑based). */
    idx_t count = 0;
    for (idx_t cand = 3; ; ++cand) {
        if (has_bordered_binary((seq_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
