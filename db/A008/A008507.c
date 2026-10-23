/*
 * A008507  Number of odd composite numbers less than n-th odd prime.
 * https://oeis.org/A008507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p-1)/2 - n where p is the nth odd prime (p =
 *            A000040(n+1))
 * Range:     u8 n <= 53, u16 n <= 7235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n + 1);            /* nth odd prime, 2 is not counted */
    seq_t half = ((seq_t)p - 1) / 2;          /* (p-1)/2 */
    return half - (seq_t)n;                   /* subtract n to count odd composites */
}
