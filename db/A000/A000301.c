/*
 * A000301  a(n) = a(n-1)*a(n-2) with a(0) = 1, a(1) = 2; also a(n) =
 *          2^Fibonacci(n).
 * https://oeis.org/A000301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2), a(0) = 1, a(1) = 2 (equivalently a(n) =
 *            2^Fibonacci(n))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 1;   /* a(0) */
    seq_t cur = 2;    /* a(1) */
    idx_t i;

    if (n == 0)
        return prev;
    for (i = 1; i < n; i++) {
        seq_t next = cur * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
