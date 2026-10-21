/*
 * A059015  Total number of 0's in binary expansions of 0, ..., n.
 * https://oeis.org/A059015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_ones_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059015: Total number of 0's in binary expansions of 0, ..., n.
   Formula: a(n) = sum_{k=0}^n (number of digits of k in binary minus popcount(k)).
   Equivalent: a(n) = total_bits(0..n) - total_ones(0..n).  */

/* standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_ones_upto.h"

/* helper: number of bits needed to represent v (v>0). For v==0 returns 1. */
static unsigned bitlen_uint64(uint64_t v)
{
    return v ? 64 - __builtin_clzll(v) : 1;
}

/* public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 by definition */
        return (seq_t)1;

    /* ---- total number of bits in all numbers 0..n ---- */
    unsigned msb = bitlen_uint64(n) - 1;       /* floor(log2 n) */
    seq_t sum_floor = 0;

    /* sum of floor(log2 k) for k = 1 .. n */
    for (unsigned m = 0; m < msb; ++m) {
        seq_t count = (seq_t)(1ULL << m);      /* numbers with floor(log2)=m */
        sum_floor += count * m;
    }
    /* contribution of the last block (numbers with floor(log2)=msb) */
    seq_t cnt_last = (seq_t)(n - ((seq_t)1 << msb) + 1);
    sum_floor += cnt_last * msb;

    /* total bits = sum_floor + n (for +1 per number) + 1 (for number 0) */
    seq_t total_bits = sum_floor + (seq_t)n + (seq_t)1;

    /* ---- total number of 1's in all numbers 0..n ---- */
    seq_t total_ones = count_ones_upto(n);

    /* zeros = total bits - total ones */
    return total_bits - total_ones;
}
