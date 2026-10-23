/*
 * A126235  Minimum length of a codeword in Huffman encoding of n symbols,
 *          where the k-th symbol has frequency k.
 * https://oeis.org/A126235
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log₂((2*(n+1))/3))
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum length of a Huffman codeword for n symbols with frequencies 1..n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute t = floor( 2*(n+1)/3 ) using a wider integer to avoid overflow */
    uint64_t t = (uint64_t)2 * ((uint64_t)n + 1) / 3;

    /* Compute floor(log₂(t)) by counting bits */
    unsigned int len = 0;
    uint64_t u = t;
    while (u) {
        ++len;
        u >>= 1;
    }

    /* floor(log₂(t)) = len - 1 */
    return (seq_t)(len - 1);
}
