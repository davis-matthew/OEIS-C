/*
 * A096982  Define a(1)=0. Then define b(1)=1, b(2)=1 and for k > 2, b(k) =
 *          (b(k-2) + b(k-1)) (mod n). This gives for each n a cyclic
 *          repetitive sequence; a(n) is the least k > 1 for the first 1 of
 *          the sequence 1,1,....
 * https://oeis.org/A096982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pisano period of Fibonacci modulo n + 1 (a(1) = 0). The
 *            Pisano period is defined as the length of the smallest positive
 *            integer t such that the Fibonacci sequence modulo n repeats with
 *            F(t)=1 and F(t+1)=1. Hence a(n) is the starting index of the
 *            second occurrence of the pair (1,1)
 * Range:     u8 n <= 49, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t prev = 1;
    seq_t curr = 1;

    for (idx_t i = 3; ; ++i) {
        seq_t next = (prev + curr) % (seq_t)n;
        prev = curr;
        curr = next;

        /* The pair (1,1) occurs at positions i-1, i.  Return the
           starting position (i-1).  The loop is guaranteed to
           terminate because every modulus has a Pisano period. */
        if (prev == 1 && curr == 1)
            return (seq_t)(i - 1);
    }
}
