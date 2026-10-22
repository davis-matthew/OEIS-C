/*
 * A000262  Number of "sets of lists": number of partitions of {1,...,n} into
 *          any number of lists, where a list means an ordered subset.
 * https://oeis.org/A000262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = (2*n-1)*a(n-1) - (n-1)*(n-2)*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 1; /* a(k-2) */
    seq_t cur = 1;  /* a(k-1) */
    idx_t k;
    if (n < 2)
        return (seq_t)1;
    for (k = 2; k <= n; k++) {
        seq_t t = (seq_t)(2 * k - 1) * cur;
        seq_t u = (seq_t)((k - 1) * (k - 2)) * prev;
        seq_t next = t - u;
        prev = cur;
        cur = next;
    }
    return cur;
}
