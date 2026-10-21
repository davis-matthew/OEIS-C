/*
 * A060317  a(n) = n * next number greater than n with same first digit as n.
 * https://oeis.org/A060317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n * next(n+), where next(n+) is the smallest integer > n
 *            that has the same first decimal digit as n. This computation
 *            uses only integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;           /* n is at most 2^64‑1 */
    uint64_t temp = n64;
    uint64_t p = 1;                       /* p = 10^(digits-1) */

    /* Extract first digit d and the power of 10 p for the most significant place. */
    while (temp >= 10) {
        temp /= 10;
        p *= 10;
    }
    uint64_t d = temp;                    /* first decimal digit of n */

    /* End of the block of numbers sharing the same first digit. */
    uint64_t block_end = d * p + (p - 1);

    seq_t next;
    if (n64 == block_end) {
        /* If n is the last number in its block, jump to the first number of the
           next block with the same first digit, which is d * 10^(digits). */
        next = (seq_t)d * (seq_t)p * 10;
    } else {
        /* Otherwise, the next number with the same first digit is simply n + 1. */
        next = (seq_t)n64 + 1;
    }

    /* Return the product n * next(n+). */
    return (seq_t)n64 * next;
}
