/*
 * A065269  Infinite binary tree inspired permutation of N: 1 -> 1, 11ab..yz
 *          -> 11ab..yz0, 10ab..y1 -> 10ab..y, 10ab..y0 -> 11AB..Y1 (where
 *          1AB..Y1 is the complement of 0ab..y0).
 * https://oeis.org/A065269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Infinite binary tree inspired permutation of N (OEIS A065269).
 * The mapping can be described by the following rules on the binary
 * representation of n:
 *
 * 1. If n == 1, a(n) = 1.
 * 2. Let len = bitlen of n (highest 1 bit index + 1).
 *    If the second most significant bit of n is 1
 *       (i.e. n starts with the pair "11"),
 *       then a(n) = n << 1   (append a 0 bit).
 *    Otherwise (n starts with the pair "10"):
 *       - If n ends with a 1, then a(n) = n >> 1  (delete the last bit).
 *       - If n ends with a 0, then let k = len - 2 (number of bits after
 *         the first two bits).  Write n as "10" + s where s has k bits.
 *         Let comp_s be the bitwise complement of s (within k bits).
 *         Then a(n) = (3 << k) | comp_s.
 *
 * This definition reproduces all listed terms of the sequence and works
 * for any n ≥ 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 1)
        return (seq_t)1;

    /* Number of bits in n */
    unsigned len = bitlen_seq_t((seq_t)n);

    /* Second most significant bit */
    bool second_bit = ((n >> (len - 2)) & 1UL) != 0;

    if (second_bit) {
        /* Rule: 11ab..yz -> 11ab..yz0 */
        return (seq_t)n << 1;
    } else {
        /* n starts with '10' */
        if ((n & 1UL) != 0) {
            /* Rule: 10ab..y1 -> 10ab..y */
            return (seq_t)(n >> 1);
        } else {
            /* Rule: 10ab..y0 -> 11AB..Y1 */
            unsigned k = len - 2;                 /* bits after the leading '10' */
            seq_t mask = ((seq_t)1 << k) - 1;     /* k-bit all‑ones mask */
            seq_t s = (seq_t)n & mask;            /* suffix s */
            seq_t comp_s = mask ^ s;              /* complement of s */
            return ((seq_t)3 << k) | comp_s;      /* '11' + comp_s */
        }
    }
}
