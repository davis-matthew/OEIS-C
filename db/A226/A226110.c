/*
 * A226110  a(1) = 0. For n > 1, a(n) = maximum distance between two
 *          consecutive elements in Collatz(3x+1) trajectory of n.
 * https://oeis.org/A226110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum absolute difference between successive terms in
 *            the Collatz (3x+1) trajectory of n
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) /* the sequence is defined for n >= 1; undefined behavior for 0 */
        return (seq_t)0;

    seq_t x = (seq_t)n;
    seq_t max_diff = (seq_t)0;

    while (x != (seq_t)1) {
        seq_t next = (x % (seq_t)2 == (seq_t)0) ? x / (seq_t)2 : (seq_t)3 * x + (seq_t)1;
        seq_t diff   = (next > x) ? next - x : x - next;
        if (diff > max_diff)
            max_diff = diff;
        x = next;
    }
    return max_diff;
}
