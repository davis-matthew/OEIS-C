/*
 * A097895  Number of compositions of n with at least 1 odd and 1 even part.
 * https://oeis.org/A097895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - F_n - (n even ? 2^(n/2-1) : 0) where F_n is the
 *            n-th Fibonacci number with F_1 = F_2 = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* total number of compositions of n */
    seq_t total = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        total *= 2;

    /* Fibonacci number F_n (F_1 = F_2 = 1) */
    seq_t odd_all = 1;   /* F_1 */
    seq_t odd_all_next = 1; /* F_2 */
    if (n > 2) {
        seq_t prev = odd_all;
        seq_t curr = odd_all_next;
        for (idx_t i = 3; i <= n; ++i) {
            seq_t next = prev + curr;
            prev = curr;
            curr = next;
        }
        odd_all_next = curr;
    }
    odd_all = odd_all_next;

    /* compositions with all parts even */
    seq_t even_all = 0;
    if ((n & 1) == 0) {          /* n is even */
        idx_t half = n >> 1;
        even_all = 1;
        for (idx_t i = 1; i <= half - 1; ++i)
            even_all *= 2;
    }

    /* desired count = total - solely odd - solely even */
    seq_t res = total - odd_all - even_all;
    return res;
}
