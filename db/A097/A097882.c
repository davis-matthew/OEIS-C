/*
 * A097882  a(n) = floor( n^2/prime(n) ).
 * https://oeis.org/A097882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2 / prime(n))
 * Range:     u8 n <= 15, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; 0 is unused */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t p = (seq_t)get_prime(n);   /* prime(n) */
    return n_sq / p;
}
