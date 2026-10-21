/*
 * A078627  Write n in binary; repeatedly sum the "digits" until reaching 1;
 *          a(n) = 1 + number of steps required.
 * https://oeis.org/A078627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 + the number of times the binary digit‐sum (popcount)
 *            must be applied to n until the result is 1. For n = 1 the value
 *            is 1
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count the steps, starting with 1 for the initial value. */
    seq_t steps = 1;
    uint64_t x = (uint64_t)n;

    /* Repeatedly replace x by the number of 1‑bits until it becomes 1. */
    while (x != 1) {
        x = (uint64_t)__builtin_popcountll(x);
        steps++;
    }

    return steps;
}
