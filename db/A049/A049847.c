/*
 * A049847  a(n) = (2*n + 1 - prevprime(2*n+1))/2.
 * https://oeis.org/A049847
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 1 - prevprime(2*n+1))/2
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For safety return 0 for smaller indices. */
    if (n < 2) {
        return 0;
    }

    /* m = 2*n + 1  is guaranteed to be odd. */
    idx_t m = 2 * n + 1;

    /* p is the largest prime strictly less than m.  The provided utility */
    /* prev_prime implements this. */
    idx_t p = prev_prime(m);

    /* The desired value is (m - p) / 2.   All arithmetic is performed in   */
    /* the generic result type seq_t to avoid overflow on wide integer types. */
    seq_t diff = (seq_t)m - (seq_t)p;
    return diff / 2;
}
