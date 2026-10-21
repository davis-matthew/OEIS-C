/*
 * A087172  Greatest Fibonacci number that does not exceed n.
 * https://oeis.org/A087172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest Fibonacci number that does not exceed n (offset
 *            1)
 * Range:     u8 n <= 375, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* out of range, not used */
    seq_t limit = (seq_t)n;      /* cast to seq_t for comparison */

    seq_t prev = 1;              /* F_1 = 1 */
    seq_t curr = 2;              /* next distinct Fibonacci number (F_2 = 2) */

    /* If n is 1, curr > limit and prev is the answer */
    if (curr > limit) return prev;

    /* Generate Fibonacci numbers until we exceed limit */
    while (curr <= limit) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return prev;
}
